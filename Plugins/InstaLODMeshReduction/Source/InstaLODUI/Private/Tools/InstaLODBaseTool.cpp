/**
 * InstaLODBaseTool.cpp (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODBaseTool.cpp
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#include "InstaLODBaseTool.h"
#include "InstaLODUIPCH.h"

#include "InstaLODModule.h"

#include "Utilities/InstaLODUtilities.h"
#include "Slate/InstaLODWindow.h"
#include "Customizations/InstaLODBaseToolCustomization.h"
#include "InstaLODImposterizeTool.h"

#include "Interfaces/IPluginManager.h"
#include "RawMesh.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "ScopedTransaction.h"
#include "MaterialUtilities.h"
#include "Engine/SkinnedAssetCommon.h"
#include "Misc/ConfigCacheIni.h"

#define LOCTEXT_NAMESPACE "InstaLODUI"

UInstaLODBaseTool::UInstaLODBaseTool()
{
	FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");
	InstaLOD = InstaLODModule.GetInstaLODInterface();

	bReplaceSelectedMeshes = false;

	SlowTaskProgress = nullptr;
	InstaLODWindow = nullptr;
	InputMesh = nullptr;
	OutputMesh = nullptr;
	MaterialData = nullptr;
	Skeleton = nullptr;
	ComponentsBoundingSphereRadius = 0.0f;
}

UInstaLODBaseTool::~UInstaLODBaseTool()
{
	InstaLODWindow = nullptr;
}

void UInstaLODBaseTool::OnNewSelection()
{
	auto* CurrentInstaLODWindow = GetInstaLODWindow();
	if (CurrentInstaLODWindow != nullptr)
	{
		MeshComponents = CurrentInstaLODWindow->GetEnabledSelectedMeshComponents(); 
	}

 	bSkeletalMeshsSelected = SkeletalMeshsSelected();
	bStaticMeshsSelected = StaticMeshsSelected();
	bSingleSkeletalMeshSelected = bSkeletalMeshsSelected && MeshComponents.Num() == 1;
}

/************************************************************************/
/* Getters / Setters													*/
/************************************************************************/

void UInstaLODBaseTool::SetInstaLODWindow(SInstaLODWindow* NewWindow)
{
	if (NewWindow)
	{
		// check if we have a specific mesh selected and bind a function to the selection delegate for updates
		InstaLODWindow = NewWindow;
		bSkeletalMeshsSelected = SkeletalMeshsSelected();
		bStaticMeshsSelected = StaticMeshsSelected();
		bSingleSkeletalMeshSelected = bSkeletalMeshsSelected && MeshComponents.Num() == 1;
		InstaLODWindow->GetOnNewSelectionDelegate().AddUObject(this, &UInstaLODBaseTool::OnNewSelection);
	}
}

bool UInstaLODBaseTool::SkeletalMeshsSelected()
{
	return InstaLODWindow ? InstaLODWindow->GetNumSkeletalMeshComponents() > 0 : false;
}

bool UInstaLODBaseTool::StaticMeshsSelected()
{
	return InstaLODWindow ? InstaLODWindow->GetNumStaticMeshsComponents() > 0 : false;
}

FString UInstaLODBaseTool::GetDefaultPackageName() const
{
 	FString PackageName;
	
	// iterate through selected actors and find first static mesh asset
	// use this static mesh path as destination package name for a merged mesh
	for(TSharedPtr<FInstaLODMeshComponent> MeshComponent : MeshComponents)
	{
		if (MeshComponent.IsValid())
		{
			if (MeshComponent->StaticMeshComponent.IsValid())
			{
				const FString MeshName = MeshComponent->StaticMeshComponent->GetStaticMesh()->GetOutermost()->GetName();
				PackageName = MeshName;
				break;
			}
			else if (MeshComponent->SkeletalMeshComponent.IsValid())
			{
				const FString MeshName = MeshComponent->SkeletalMeshComponent->GetSkeletalMeshAsset()->GetOutermost()->GetName();
				PackageName = MeshName;
				break;
			}
		}
	}

	// Still no PackageName? Create one
	if (PackageName.IsEmpty())
	{
		PackageName = MakeUniqueObjectName(NULL, UPackage::StaticClass(), *PackageName).ToString();
	}
	
	// remove common mesh type prefixes
	PackageName = PackageName.Replace(TEXT("SM_"), TEXT(""))
	.Replace(TEXT("T_"), TEXT(""))
	.Replace(TEXT("M_"), TEXT(""))
	.Replace(TEXT("ML_"), TEXT(""))
	.Replace(TEXT("SK_"), TEXT(""));
	
 	return PackageName;
}

bool UInstaLODBaseTool::IsSelectedMeshesLODSValid()
{
	TArray<FString> AffectedMeshesNames;
	TArray<int32> IndicesOfMeshesWithInvalidNumberOfLODS;
	for (int32 Index = 0; Index < MeshComponents.Num(); ++Index)
	{
		if (const UStaticMeshComponent* StaticMeshComponent = MeshComponents[Index]->StaticMeshComponent.Get())
		{
			if (TargetLODIndex > StaticMeshComponent->GetStaticMesh()->GetNumSourceModels() - 1)
			{
				AffectedMeshesNames.Add(StaticMeshComponent->GetReadableName());
				IndicesOfMeshesWithInvalidNumberOfLODS.Add(Index);
			}
			continue;
		}

		if (const USkeletalMeshComponent* SkeletalMeshComponent = MeshComponents[Index]->SkeletalMeshComponent.Get())
		{
			if (TargetLODIndex > SkeletalMeshComponent->GetSkeletalMeshAsset()->GetLODNum() - 1)
			{
				AffectedMeshesNames.Add(SkeletalMeshComponent->GetReadableName());
				IndicesOfMeshesWithInvalidNumberOfLODS.Add(Index);
			}
			continue;
		}
	}

	if (IndicesOfMeshesWithInvalidNumberOfLODS.Num() == 0)
		return true;

	const FText WarningMessageTitle = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODInvalidLODIndexMessageTitle", "InstaLOD: Invalid target LOD index.");
	const FText SelectionInvalidMessageFormat = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODInvalidLODIndexMessageAbortingInformation", "None of the selected meshes contains LOD at index {LODINDEX}. Please select a valid LOD index and try again.");
	const FText SelectedMeshInvalidMessageFormat = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODInvalidLODIndexMessageAbortingInformationForSingleMesh", "The selected mesh does not contain LOD at index {LODINDEX}. Please select a valid LOD index and try again.");
	const FText InvalidMeshesMessageFormat = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODInvalidLODIndexMessageContent", "The following {SELECTIONTEXT} not contain an LOD in the specified index.These meshes will be excluded from the operation :\n\n{INVALIDMESHES}.\n\nDo you want to continue?");

	const uint32 ValidSelectedMeshesCount = MeshComponents.Num() - IndicesOfMeshesWithInvalidNumberOfLODS.Num();
	if (ValidSelectedMeshesCount == 0)
	{
		const FText MessageContent = IndicesOfMeshesWithInvalidNumberOfLODS.Num() == 1 ? FText::FormatNamed(SelectedMeshInvalidMessageFormat, TEXT("LODINDEX"), TargetLODIndex) : FText::FormatNamed(SelectionInvalidMessageFormat, TEXT("LODINDEX"), TargetLODIndex);
		FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, MessageContent, WarningMessageTitle);
		return false;
	}

	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("SELECTIONTEXT"), IndicesOfMeshesWithInvalidNumberOfLODS.Num() == 1 ? NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODMeshInvalidLODIndexSelectionTextSingular", "Mesh does") : NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODMeshInvalidLODIndexSelectionTextPlural", "Meshes do"));
	Arguments.Add(TEXT("INVALIDMESHES"), FText::FromString(FString::Join(AffectedMeshesNames, TEXT(" | "))));
	const FText WarningMessageContent = FText::Format(InvalidMeshesMessageFormat, Arguments);

	const EAppReturnType::Type UserChoice = FInstaLODModule::MessageDialogOpen(EAppMsgType::OkCancel, WarningMessageContent, WarningMessageTitle);

	if (UserChoice != EAppReturnType::Ok)
		return false;

	for (int32 index = IndicesOfMeshesWithInvalidNumberOfLODS.Num() - 1; index >= 0; --index)
	{
		MeshComponents.RemoveAt(IndicesOfMeshesWithInvalidNumberOfLODS[index]);
	}

	check(MeshComponents.Num() > 0);

	return true;
}

/************************************************************************/
/* Tool Interface														*/
/************************************************************************/

bool UInstaLODBaseTool::IsMeshOperationExecutable(FText* OutErrorText) const
{
	// ensure geometry is selected
	if (MeshComponents.Num() == 0)
	{
		if (OutErrorText != nullptr)
		{
			*OutErrorText = NSLOCTEXT(LOCTEXT_NAMESPACE, "OperationFailed_EmptySelection", "No geometry selected or enabled for processing.");
		}
		return false;
	}
	
	// if a transform freeze is require and a multi selection is enabled
	// we can only proceed if the result usage is set to 'Save as new asset
	if (IsFreezingTransformsForMultiSelection() && MeshComponents.Num() > 1 &&
		ResultUsage != EInstaLODResultUsage::InstaLOD_NewAsset)
	{
		if (OutErrorText != nullptr)
		{
			*OutErrorText = NSLOCTEXT(LOCTEXT_NAMESPACE, "OperationFailed_ResultUsageMustBeNewAsset",
									  "Processing multiple meshes in the current operation requires 'Save as' set to 'Save as new asset'");
		}
		return false;
	}
	
	// NOTE: if the result usage is set to append, we have to ensure that
	// the user only selected each skeletal/static mesh resource exactly once
	// otherwise we might append multiple results to a unique input mesh
	if (ResultUsage == EInstaLODResultUsage::InstaLOD_AppendToLOD ||
		ResultUsage == EInstaLODResultUsage::InstaLOD_ReplaceLOD)
	{
		TSet<UObject*> UniqueComponents;
		
		bool bHasDuplicate = false;
		
		TArray<UStaticMesh*> StaticMeshesWithLODGroup;
		
		for(TSharedPtr<FInstaLODMeshComponent> MeshComponent : MeshComponents)
		{
			if (MeshComponent.IsValid())
			{
				if (MeshComponent->StaticMeshComponent.IsValid())
				{
					UStaticMesh *const StaticMesh = MeshComponent->StaticMeshComponent->GetStaticMesh();
					
					if (UniqueComponents.Contains(StaticMesh))
					{
						bHasDuplicate = true;
						break;
					}
					UniqueComponents.Add(StaticMesh);
					
					if (StaticMesh->LODGroup != NAME_None)
						StaticMeshesWithLODGroup.Add(StaticMesh);
				}
				else if (MeshComponent->SkeletalMeshComponent.IsValid())
				{
					if (UniqueComponents.Contains(MeshComponent->SkeletalMeshComponent->GetSkeletalMeshAsset()))
					{
						bHasDuplicate = true;
						break;
					}
					UniqueComponents.Add(MeshComponent->SkeletalMeshComponent->GetSkeletalMeshAsset());
				}
			}
		}
		
		if (bHasDuplicate)
		{
			if (OutErrorText)
			{
				*OutErrorText = NSLOCTEXT(LOCTEXT_NAMESPACE, "SameMeshes",
										  "When 'Save as' is set to 'Append To LOD Chain' or 'Replace LOD at target index' all meshes assigned to the components in the selection have to be unique.");
			}
			return false;
		}
		
		// there is at least one mesh with a LOD group, tell the user that proceeding
		// will force us to bake the LODs in the mesh packages
		if (StaticMeshesWithLODGroup.Num() > 0)
		{
			if (OutErrorText == nullptr)
			{
				FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "MeshesWithLODGroupTitle", "Mesh LOD groups");
				FString WarningMessage = NSLOCTEXT(LOCTEXT_NAMESPACE, "MeshesWithLODGroupInfo",
												 "Appending or replacing a LOD on a static mesh with a LOD group will require InstaLOD to bake all LODs in the mesh's package and set the LOD group to 'None'.\n"
												 "The following meshes have LOD groups set:\n\n").ToString();

				
				for(TSharedPtr<FInstaLODMeshComponent> MeshComponent : MeshComponents)
				{
					WarningMessage += " - " + MeshComponent->StaticMeshComponent->GetStaticMesh()->GetOutermost()->GetName() + "\n";
				}
				
				WarningMessage += NSLOCTEXT(LOCTEXT_NAMESPACE, "MeshesWithLODGroupQuestion", "\nDo you want to bake all LODs in the mesh packages?").ToString();

				EAppReturnType::Type Return = FInstaLODModule::MessageDialogOpen(EAppMsgType::YesNo, FText::FromString(WarningMessage), Title);
			
				if (Return == EAppReturnType::No)
					return false;
			}
			else
			{
				*OutErrorText = NSLOCTEXT(LOCTEXT_NAMESPACE, "MeshesWithLODGroupError", "Appending or replacing a LOD on a static mesh requires the LOD group field set to 'None'.");
				return false;
			}
		}
	}

	return true;
}

void UInstaLODBaseTool::OnMeshOperationError(const FText& Message) const
{
	const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "OptimizeTool_Title", "Operation failed!");
	FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, Message.IsEmpty() ? NSLOCTEXT(LOCTEXT_NAMESPACE, "OptimizeTool_DefaultErrorMessage", "The operation has failed!") : Message, Title);
	UInstaLODUtilities::WriteInstaLODMessagesToLog(GetInstaLODInterface());
}

void UInstaLODBaseTool::ExecuteMeshOperation()
{
	// clear message log
	GetInstaLODInterface()->GetInstaLOD()->ClearMessageLog();
	
	if (!GetInstaLODWindow())
	{
		OnMeshOperationError(NSLOCTEXT(LOCTEXT_NAMESPACE, "OperationFailed_NoWindow", "Couldn't locate InstaLOD window."));
		return;
	}
	
	// grab list of meshes for this operation
	MeshComponents = GetInstaLODWindow()->GetEnabledSelectedMeshComponents();
	
	// ensure we can execute the current configuration
	if (!IsMeshOperationExecutable(nullptr))
	{
		FText ErrorMessage;
		IsMeshOperationExecutable(&ErrorMessage);
		OnMeshOperationError(ErrorMessage);
		return;
	}
	
	// Warn the user if multiple assets are being generated and no output path is set.
	if (ResultUsage == EInstaLODResultUsage::InstaLOD_NewAsset && NewAssetsOutputDirectory.Path.IsEmpty() && IsGeneratingMultipleAssets())
	{
		const EAppReturnType::Type Result = FInstaLODModule::MessageDialogOpen(EAppMsgType::YesNo, NSLOCTEXT(LOCTEXT_NAMESPACE, "MultipleAssets_Warning",
			"Multiple assets will be created with the current Mesh Operation settings, but the Output Path setting is empty. "
			"Each asset will be saved through a separate popup window, which might interrupt your work.\n\n"
			"Do you want to proceed?"),
			NSLOCTEXT(LOCTEXT_NAMESPACE, "MultipleAssets_Title_Warning", "Multiple Assets Generated"));

		if (Result == EAppReturnType::Type::No || Result == EAppReturnType::Type::Cancel)
			return;
	}

	if (ResultUsage == EInstaLODResultUsage::InstaLOD_ReplaceLOD && !IsSelectedMeshesLODSValid())
		return;

	OnMeshOperationBegin();

	SlowTaskProgress = new FScopedSlowTask(/*InAmountOfWork: */100.0f, NSLOCTEXT(LOCTEXT_NAMESPACE, "OptimizeOperation", "Processing Mesh Operation"));
	SlowTaskProgress->MakeDialog(/*bShowCancelButton: */false);

	OnMeshOperationExecute(false);

	OnMeshOperationFinalize();
}

void UInstaLODBaseTool::OnMeshOperationBegin()
{
	// -----------------------
	// must run on main thread
	// -----------------------

	check(InputMesh == nullptr);
	check(BakeMesh == nullptr);
	check(OutputMesh == nullptr);
	check(MaterialData == nullptr);

	InputMesh = GetInstaLODInterface()->AllocInstaLODMesh();
	OutputMesh = GetInstaLODInterface()->AllocInstaLODMesh();

	const bool bFreezeTransform = (IsFreezingTransformsForMultiSelection() && MeshComponents.Num() > 1) || IsWorldTransformRequired();
	
	FBoxSphereBounds BoundingBox = FBoxSphereBounds(FVector::ZeroVector, FVector::ZeroVector, 0.0f);
	
	if (IsMaterialDataRequired())
	{
		// alloc material data
		MaterialData = GetInstaLODInterface()->GetInstaLOD()->AllocMaterialData();
		
		// create merge data array that contains instalod meshes and their original component
		TArray<InstaLODMergeData> MergeData;
		for (TSharedPtr<FInstaLODMeshComponent> MeshComponent : MeshComponents)
		{
			InstaLODMergeData MergeDataEntry;
			MergeDataEntry.Component = MeshComponent.Get();
			MergeDataEntry.InstaLODMesh = GetInstaLODInterface()->AllocInstaLODMesh();
			
			UInstaLODUtilities::GetInstaLODMeshFromMeshComponent(GetInstaLODInterface(), MeshComponent, MergeDataEntry.InstaLODMesh, 0, false);

			if (bFreezeTransform)
			{
				UInstaLODUtilities::TransformInstaLODMesh(MergeDataEntry.InstaLODMesh, MergeDataEntry.Component->GetComponent()->GetComponentTransform(), /*LocalToWorld:*/true);
			}
			
			if (BoundingBox.SphereRadius == 0)
			{
				BoundingBox = MergeDataEntry.Component->GetComponent()->CalcBounds(MergeDataEntry.Component->GetComponent()->GetComponentTransform());
			}
			else
			{
				BoundingBox = BoundingBox + MergeDataEntry.Component->GetComponent()->CalcBounds(MergeDataEntry.Component->GetComponent()->GetComponentTransform());
			}
			MergeData.Add(MergeDataEntry);
		}
		
		ComponentsBoundingSphereRadius = BoundingBox.SphereRadius;

		// the create material data method will bake out all materials into flat textures
		// it will also remap all face material indices to point to the right index in our material data instance
		// if required by the material graph (or vertex color baking) new UVs will be generated as well
		TArray<UMaterialInterface*> UniqueMaterials;
		UInstaLODUtilities::CreateMaterialData(GetInstaLODInterface(), MergeData, MaterialData, GetMaterialProxySettings(), UniqueMaterials);

		if (IsBakeMeshSupported())
		{
			const FString BakeMeshSuffix = GetBakeMeshSuffix();

			if (!BakeMeshSuffix.IsEmpty())
			{
				BakeMesh = GetInstaLODInterface()->AllocInstaLODMesh();
			}

			// aggregate all meshes
			for (InstaLODMergeData& MergeDataEntry : MergeData)
			{
				check(MergeDataEntry.InstaLODMesh);

				ON_SCOPE_EXIT
				{
					// dealloc mesh
					GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(MergeDataEntry.InstaLODMesh);
				};

				const FString ComponentName = MergeDataEntry.Component->GetDisplayName();

				// aggregate all bake meshes
				if (!BakeMeshSuffix.IsEmpty() && ComponentName.EndsWith(BakeMeshSuffix))
				{
					BakeMesh->AppendMesh(MergeDataEntry.InstaLODMesh);
					continue;
				}

				// aggregate all bake meshes
				if (CanAppendMeshToInput(*MergeDataEntry.Component, MergeDataEntry.InstaLODMesh, &UniqueMaterials))
				{
					InputMesh->AppendMesh(MergeDataEntry.InstaLODMesh);
				}
			}
		}
		else
		{
			// aggregate all meshes
			for (InstaLODMergeData& MergeDataEntry : MergeData)
			{
				check(MergeDataEntry.InstaLODMesh);

				if (CanAppendMeshToInput(*MergeDataEntry.Component, MergeDataEntry.InstaLODMesh, &UniqueMaterials))
				{
					InputMesh->AppendMesh(MergeDataEntry.InstaLODMesh);
				}

				// dealloc mesh
				GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(MergeDataEntry.InstaLODMesh);
			}
		}
	}
	else
	{
		// material data is not required, we can just aggregate all meshes
		InstaLOD::IInstaLODMesh *const TempMesh = GetInstaLODInterface()->AllocInstaLODMesh();

		for (TSharedPtr<FInstaLODMeshComponent> MeshComponent : MeshComponents)
		{
			// NOTE: in case we have only one single skeletal mesh selected and a bake pose, we go directly to GetInstaLODMeshFromSkeletalMeshComponent
			if(!bSingleSkeletalMeshSelected || GetBakePose() == nullptr)
			{
				UInstaLODUtilities::GetInstaLODMeshFromMeshComponent(GetInstaLODInterface(), MeshComponent, TempMesh);
			}
			else
			{
				UInstaLODUtilities::GetInstaLODMeshFromSkeletalMeshComponent(InstaLOD, MeshComponent->SkeletalMeshComponent.Get(), TempMesh, 0, GetBakePose());
			}
			
			if (bFreezeTransform)
			{
				UInstaLODUtilities::TransformInstaLODMesh(TempMesh, MeshComponent->GetComponent()->GetComponentTransform(), /*LocalToWorld:*/true);
			}
			
			if (CanAppendMeshToInput(*MeshComponent, TempMesh, nullptr))
			{
				InputMesh->AppendMesh(TempMesh);
			}
		}

		// NOTE: in case the next operation may need the skeleton
		if (bSingleSkeletalMeshSelected && MeshComponents.Num() == 1)
		{
			// Get SkeletalMesh 
			TSharedPtr<FInstaLODMeshComponent> MeshComponent = MeshComponents[0];
			UEBoneIndexToInstaLODBoneIndexAndName.Empty();

			bool bConversionSuccess = false;

			if (MeshComponent->SkeletalMeshComponent.IsValid())
			{
				Skeleton = GetInstaLODInterface()->AllocInstaLODSkeleton();
				bConversionSuccess = GetInstaLODInterface()->ConvertReferenceSkeletonToInstaLODSkeleton(MeshComponent->SkeletalMeshComponent->GetSkeletalMeshAsset()->GetRefSkeleton(), Skeleton, UEBoneIndexToInstaLODBoneIndexAndName);
			}
		}

		GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(TempMesh);
	}
}

void UInstaLODBaseTool::OnMeshOperationExecute(bool bIsAsynchronous)
{
	// --------------------------
	// can be run on child thread
	// --------------------------
}

void UInstaLODBaseTool::OnMeshOperationFinalize()
{
	// -----------------------
	// must run on main thread
	// -----------------------
	ON_SCOPE_EXIT
	{
		delete SlowTaskProgress;
		SlowTaskProgress = nullptr;
	};

	if (IsMeshOperationSuccessful())
	{
		uint64 FaceIndexCount;
		uint32* const SubmeshIndices = OutputMesh->GetFaceSubMeshIndices(&FaceIndexCount);
		const uint32 OutputSubMeshCount = FaceIndexCount == 0 || !IsGeneratingMultipleAssets() ? 1 : MeshComponents.Num();
		const int32 FinalLODIndex = ResultUsage == EInstaLODResultUsage::InstaLOD_ReplaceLOD ? TargetLODIndex : -1;
		
		UMaterialInstanceConstant* BakeMaterial = nullptr; // If set this material will be set onto any geometry created
		TArray<UObject*> AssetsToSync; // Used to save any object that is created during the following process

		bool bIsImposter = false;
		bool bIsVistaImposter = false;
		bool bIsFlipbookImposter = false;
		bool bIsVistaPostTransformsRequired = false;
		FTransform VistaTransform = FTransform::Identity;
		UInstaLODImposterizeTool *const ImposterizeTool = Cast<UInstaLODImposterizeTool>(this);

		if (ImposterizeTool != nullptr )
		{
			bIsFlipbookImposter = ImposterizeTool->IsFlipbookImposter();
			bIsVistaImposter = ImposterizeTool->IsVistaImposter();
			VistaTransform = ImposterizeTool->VistaTransform;
			bIsVistaPostTransformsRequired = ImposterizeTool->VistaDistance < 1.0f;
			bIsImposter = true;

			if (ImposterizeTool->TargetMesh != nullptr)
			{
				GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(ImposterizeTool->TargetMesh);
			}
		}

		// create output material
		if (MaterialData != nullptr)
		{
			// Get save path

			// NOTE: We are suggesting a name with the "MI" prefix to identify possible collisions, but remove it as Unreal automatically adds the same prefix when creating the material.
			FString SaveMaterialPath = GetAssetOutputPathForType<UMaterialInstanceConstant>(TEXT("MI_") + FPackageName::GetShortName(GetDefaultPackageName()));

			// Remove the "MI" prefix only from the filename.
			const FString MaterialFilename = FPaths::GetCleanFilename(SaveMaterialPath).Replace(TEXT("MI_"), TEXT(""));

			const FString MaterialSaveDirectory = FPaths::GetPath(SaveMaterialPath);
			SaveMaterialPath = FPaths::Combine(MaterialSaveDirectory, MaterialFilename);

			if (SaveMaterialPath.IsEmpty())
			{
				UE_LOG(LogInstaLOD, Warning, TEXT("User aborted material asset creation."));
			}
			else
			{
				FFlattenMaterial FlattenMaterial;
				// store merge material result in output FFlattenMaterial
				GetInstaLODInterface()->ConvertInstaLODMaterialToFlattenMaterial(GetBakeMaterial(), FlattenMaterial, GetMaterialProxySettings());
				
				const FString AssetBaseName = FPackageName::GetShortName(SaveMaterialPath);
				const FString AssetBasePath = FPackageName::GetLongPackagePath(SaveMaterialPath) + TEXT("/");
				
				// optimize the flattened material
				FMaterialUtilities::OptimizeFlattenMaterial(FlattenMaterial);

				// create a proxy material instance
				BakeMaterial = UInstaLODUtilities::CreateFlattenMaterialInstance(FlattenMaterial, GetMaterialProxySettings(), SaveMaterialPath, AssetsToSync, bIsFlipbookImposter);
				
				if (BakeMaterial == nullptr)
				{
					OnMeshOperationError(NSLOCTEXT(LOCTEXT_NAMESPACE, "OperationFailed_NoPackageCreated", "Failed to create new uasset for material data."));
				}
				else
				{
					// set static lighting flag if necessary
					static const auto AllowStaticLightingVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.AllowStaticLighting"));
					const bool bAllowStaticLighting = (!AllowStaticLightingVar || AllowStaticLightingVar->GetValueOnGameThread() != 0);
					if (bAllowStaticLighting)
					{
						BakeMaterial->CheckMaterialUsage(MATUSAGE_StaticLighting);
					}
					
					FStaticParameterSet StaticParameters;
					BakeMaterial->GetStaticParameterValues(StaticParameters);
					bool bStaticParametersDirty = false;
					
					// save out all additional materials
					for(int TexturePageIndex=0; TexturePageIndex<(int)GetBakeMaterial()->GetTexturePageCount(); TexturePageIndex++)
					{
						InstaLOD::IInstaLODTexturePage *const TexturePage = GetBakeMaterial()->GetTexturePageAtIndex(TexturePageIndex);
						check(TexturePage);
						
						const FString TexturePageName = FString(UTF8_TO_TCHAR(TexturePage->GetName()));
						
						// ignore internal UE_ texture pages, they're part of our proxy material instance
						if (TexturePageName.StartsWith(TEXT("UE_")))
							continue;
						
						// skip bake texture pages that we can directly hookup to the material
						if (TexturePageName.Equals(TEXT("NormalTangentSpace")) ||
							TexturePageName.Equals(TEXT("Opacity")) ||
							TexturePageName.Equals(TEXT("AmbientOcclusion")))
							continue;
						
						const FString SaveTexturePagePath = GetAssetOutputPathForType<UTexture>(TEXT("T_") + AssetBaseName + TEXT("_") + TexturePageName);
						
						UTexture *const Texture = UInstaLODUtilities::ConvertInstaLODTexturePageToTexture(TexturePage, SaveTexturePagePath);
						
						if (!Texture)
						{
							OnMeshOperationError(NSLOCTEXT(LOCTEXT_NAMESPACE, "OperationFailed_NoPackageCreated", "Failed to create new uasset for texture data."));
							continue;
						}
						
						if (TexturePageName.Equals(TEXT("Displacement")))
						{
							BakeMaterial->SetTextureParameterValueEditorOnly(TEXT("ParallaxTexture"), Texture);
							BakeMaterial->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo("UseParallaxTexture"), /*Value:*/ true);
							bStaticParametersDirty = true;
						}
						else if (TexturePageName.Equals(TEXT("BentNormals")))
						{
							BakeMaterial->SetTextureParameterValueEditorOnly(TEXT("BentNormalsTexture"), Texture);
							BakeMaterial->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo("UseBentNormals"), /*Value:*/ true); 
							bStaticParametersDirty = true;
						}
						else if (TexturePageName.Equals(TEXT("NormalObjectSpace")))
						{
							BakeMaterial->SetTextureParameterValueEditorOnly(TEXT("ObjectSpaceNormalTexture"), Texture);
							BakeMaterial->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo("UseObjectSpaceNormalMap"), /*Value:*/ true);
							bStaticParametersDirty = true;
						}

						UE_LOG(LogInstaLOD, Warning,TEXT("%s"),*TexturePageName);
						AssetsToSync.Add(Texture);
					}
					
					if (bIsFlipbookImposter)
					{
						BakeMaterial->SetScalarParameterValueEditorOnly(TEXT("FramesPerAxis"), ImposterizeTool->GetFlipbookFramesPerAxisCount());
						BakeMaterial->SetScalarParameterValueEditorOnly(TEXT("SpriteSize"), ComponentsBoundingSphereRadius);

						bStaticParametersDirty = true;
					}
					
					FinalizeBakeMaterial(BakeMaterial);
					
					if (bStaticParametersDirty)
					{
						// force update of the static permutations
						BakeMaterial->UpdateStaticPermutation(StaticParameters);
						BakeMaterial->InitStaticPermutation();
						BakeMaterial->PostEditChange();
					}
				}
			}
		}
		
		static const FString kInstaLODMeshSuffix = TEXT("_InstaLOD");

		// if the output submesh count is 1 our operation created as single output mesh
		// in this case, we should only be operating in "NewAsset" mode
		// and it will always be a static mesh
		if (OutputSubMeshCount == 1 && MeshComponents.Num() > 1)
		{
			check(ResultUsage == EInstaLODResultUsage::InstaLOD_NewAsset);
			
			const FString SaveObjectPath = GetAssetOutputPathForType<UStaticMesh>(FString::Printf(TEXT("SM_%s%s"), *FPackageName::GetShortName(GetDefaultPackageName()), *kInstaLODMeshSuffix));
			UObject* SavedObject = nullptr;
			
			if (SaveObjectPath.IsEmpty())
			{
				UE_LOG(LogInstaLOD, Warning, TEXT("User aborted asset creation."));
			}
			else
			{
				if (bIsFlipbookImposter)
				{
					InstaLOD::IInstaLODMesh *const Plane = UInstaLODUtilities::CreatePlane(GetInstaLODInterface(), FVector2d(ComponentsBoundingSphereRadius, ComponentsBoundingSphereRadius));
					SavedObject = UInstaLODUtilities::SaveInstaLODMeshToStaticMeshAsset(GetInstaLODInterface(), Plane, SaveObjectPath);
					GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(Plane);
				}
				else
				{
					SavedObject = UInstaLODUtilities::SaveInstaLODMeshToStaticMeshAsset(GetInstaLODInterface(), OutputMesh, SaveObjectPath);
				}
			}
			
			if (SavedObject != nullptr)
			{
				UStaticMesh *const StaticMesh = Cast<UStaticMesh>(SavedObject);
				check(StaticMesh != nullptr);
				if (bIsVistaImposter && bIsVistaPostTransformsRequired)
				{
					UInstaLODUtilities::ApplyPostTransformForVista(StaticMesh, VistaTransform);
				}
				FVector ActorLocation(0, 0, 0);
				if (IsFreezingTransformsForMultiSelection() && PivotPosition != EInstaLODPivotPosition::InstaLOD_Default)
				{
					FVector Position;
					bool bLimitedRange = true;
					switch (PivotPosition)
					{
					case EInstaLODPivotPosition::InstaLOD_Center:
						Position.Set(0.0, 0.0, 0.0);
						break;

					case EInstaLODPivotPosition::InstaLOD_Top:
						Position.Set(0.0, 0.0, 1.0);
						break;

					case EInstaLODPivotPosition::InstaLOD_Bottom:
						Position.Set(0.0, 0.0, -1.0);
						break;

					case EInstaLODPivotPosition::InstaLOD_Custom:
						Position = WorldSpacePivotPosition;
						bLimitedRange = false;
						break;

					case EInstaLODPivotPosition::InstaLOD_CustomLimited:
						// rescale the vector to be in the interval [-1, 1]
						Position = (BoundingBoxPivotPosition - 0.5) * 2;
						break;

					default:
						break;
					}
					ActorLocation = UInstaLODUtilities::CustomizePivotPosition(StaticMesh, Position, bLimitedRange);
				}

				// update the material
				if (BakeMaterial != nullptr)
				{
					StaticMesh->GetStaticMaterials().Empty();
					StaticMesh->GetStaticMaterials().Add(FStaticMaterial(BakeMaterial));
					StaticMesh->Build();
					StaticMesh->PostEditChange();
				}
				
				// deactivate all original actors
				if (bReplaceSelectedMeshes)
				{
					for(TSharedPtr<FInstaLODMeshComponent>& component : MeshComponents)
					{
						if (component->StaticMeshComponent.IsValid())
						{
							component->StaticMeshComponent->SetVisibility(false);
						}
						else if (component->SkeletalMeshComponent.IsValid())
						{
							component->SkeletalMeshComponent->SetVisibility(false);
						}
					}
					
					// insert new static mesh actor into the scene at origin (0,0,0)
					// with our new static mesh asset
					UWorld *World = MeshComponents[0]->StaticMeshComponent.IsValid() ? MeshComponents[0]->StaticMeshComponent->GetWorld() : MeshComponents[0]->SkeletalMeshComponent->GetWorld();
					ULevel *Level = MeshComponents[0]->StaticMeshComponent.IsValid() ? MeshComponents[0]->StaticMeshComponent->GetComponentLevel() : MeshComponents[0]->SkeletalMeshComponent->GetComponentLevel();
					{
						const FScopedTransaction Transaction(NSLOCTEXT(LOCTEXT_NAMESPACE, "SpawnInstaLODMeshActor", "Spawn InstaLOD Mesh Actor"));
						
						FRotator ActorRotation(ForceInit);
						
						FActorSpawnParameters Params;
						Params.OverrideLevel = Level;
						
						Level->Modify();
						
						AStaticMeshActor* InstaLODActor = World->SpawnActor<AStaticMeshActor>(ActorLocation, ActorRotation, Params);
						InstaLODActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
						InstaLODActor->SetActorLabel(StaticMesh->GetName());
						World->UpdateCullDistanceVolumes(InstaLODActor, InstaLODActor->GetStaticMeshComponent());
					}
				}
				
				AssetsToSync.Add(SavedObject);
			}
		}
		else
		{
			InstaLOD::IInstaLODMesh *const TempMesh = GetInstaLODInterface()->AllocInstaLODMesh();
			check(TempMesh);
			
			for (uint32 SubmeshIndex=0; SubmeshIndex<OutputSubMeshCount; SubmeshIndex++)
			{
				if (!MeshComponents.IsValidIndex(SubmeshIndex) || !MeshComponents[SubmeshIndex].IsValid())
					continue;
				
				TSharedPtr<FInstaLODMeshComponent> MeshComponent = MeshComponents[SubmeshIndex];
				
				OutputMesh->ExtractSubMesh(SubmeshIndex, TempMesh);

				if (IsWorldTransformRequired() && (!bIsVistaImposter || ResultUsage != EInstaLODResultUsage::InstaLOD_NewAsset))
				{
					UInstaLODUtilities::TransformInstaLODMesh(TempMesh, MeshComponent->GetComponent()->GetComponentTransform(), /*LocalToWorld:*/false);
				}

				if (ResultUsage == EInstaLODResultUsage::InstaLOD_NewAsset)
				{
					FString SaveObjectPath;

					if (MeshComponent->StaticMeshComponent.IsValid())
					{
						const FString SavePackageName = FPaths::GetCleanFilename(MeshComponent->StaticMeshComponent->GetStaticMesh()->GetOutermost()->GetName() + kInstaLODMeshSuffix);
						SaveObjectPath = GetAssetOutputPathForType<UStaticMesh>(SavePackageName);
					}
					else if (MeshComponent->SkeletalMeshComponent.IsValid())
					{
						const FString SavePackageName = FPaths::GetCleanFilename(MeshComponent->SkeletalMeshComponent->GetSkeletalMeshAsset()->GetOutermost()->GetName() + kInstaLODMeshSuffix);
						SaveObjectPath = GetAssetOutputPathForType<USkeletalMesh>(SavePackageName);
					}

					if (SaveObjectPath.IsEmpty())
					{
						UE_LOG(LogInstaLOD, Warning, TEXT("Skipped the creation of asset: %s"), *GetDefaultPackageName());
						continue;
					}

					UObject* SavedObject = nullptr;
					if (bIsFlipbookImposter)
					{
						InstaLOD::IInstaLODMesh *const Plane = UInstaLODUtilities::CreatePlane(GetInstaLODInterface(), FVector2d(ComponentsBoundingSphereRadius, ComponentsBoundingSphereRadius));
						SavedObject = UInstaLODUtilities::SaveInstaLODMeshToStaticMeshAsset(GetInstaLODInterface(), Plane, SaveObjectPath);
						GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(Plane);
					}
					else
					{
						SavedObject = UInstaLODUtilities::SaveInstaLODMeshToDuplicateAsset(GetInstaLODInterface(), MeshComponent, TempMesh, SaveObjectPath, BakeMaterial != nullptr);
					}
					
					if (!SavedObject)
					{
						OnMeshOperationError(NSLOCTEXT(LOCTEXT_NAMESPACE, "OperationFailed_NoPackageCreated", "Failed to create new uasset for processed mesh data."));
						continue;
					}
					if (Cast<UStaticMesh>(SavedObject) && bIsVistaImposter && bIsVistaPostTransformsRequired)
					{
						UStaticMesh *const StaticMesh = Cast<UStaticMesh>(SavedObject);
						UInstaLODUtilities::ApplyPostTransformForVista(StaticMesh, VistaTransform);
					}

					if (BakeMaterial != nullptr)
					{
						if (Cast<UStaticMesh>(SavedObject))
						{
							UStaticMesh *const StaticMesh = Cast<UStaticMesh>(SavedObject);
							StaticMesh->GetStaticMaterials().Empty();
							StaticMesh->GetStaticMaterials().Add(FStaticMaterial(BakeMaterial));
							StaticMesh->Build();
							StaticMesh->PostEditChange();
						}
						else if (Cast<USkeletalMesh>(SavedObject))
						{
							USkeletalMesh *const SkeletalMesh = Cast<USkeletalMesh>(SavedObject);
							SkeletalMesh->GetMaterials().Empty();
							SkeletalMesh->GetMaterials().Add(FSkeletalMaterial(BakeMaterial));
							SkeletalMesh->PostEditChange();
						}
					}

					AssetsToSync.Add(SavedObject);

					if (bReplaceSelectedMeshes)
					{
						const bool bRemoveOverrideMaterials = BakeMaterial != nullptr ? true: false;
						UInstaLODUtilities::UpdateMeshOfMeshComponent(MeshComponent, SavedObject, bRemoveOverrideMaterials);
					}
				}
				else
				{
					// save the extracted data to the original uasset's rawmesh
					if (bIsFlipbookImposter)
					{
						InstaLOD::IInstaLODMesh *const Plane = UInstaLODUtilities::CreatePlane(GetInstaLODInterface(), FVector2d(ComponentsBoundingSphereRadius, ComponentsBoundingSphereRadius));
						UInstaLODUtilities::InsertLODToMeshComponent(GetInstaLODInterface(), MeshComponent, Plane, FinalLODIndex, BakeMaterial);
						GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(Plane);
					}
					else
					{
						UInstaLODUtilities::InsertLODToMeshComponent(GetInstaLODInterface(), MeshComponent, TempMesh, FinalLODIndex, BakeMaterial);
					}
				}
			}
			
			if (TempMesh != nullptr)
			{
				GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(TempMesh);
			}
		}

		// pending assets to save?
		if (AssetsToSync.Num() > 0)
		{
			// Now sync the saved meshes with the Content Browser
			FAssetRegistryModule& AssetRegistry = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

			for (int32 AssetIndex = 0; AssetIndex < AssetsToSync.Num(); AssetIndex++)
			{
				AssetRegistry.AssetCreated(AssetsToSync[AssetIndex]);
				GEditor->BroadcastObjectReimported(AssetsToSync[AssetIndex]);
			}

			// Also notify the content browser that the new assets exists
			FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
			ContentBrowserModule.Get().SyncBrowserToAssets(AssetsToSync, true, true);
		}

		// Set the progress bar to complete
		SlowTaskProgress->CompletedWork = 100.0f;
		SlowTaskProgress->EnterProgressFrame(0.0f);
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 2
		SlowTaskProgress->ForceRefresh();
#endif

		const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "OptimizeTool_SuccessfulMessage", "Operation successful!");
		FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, NSLOCTEXT(LOCTEXT_NAMESPACE, "OptimizeTool_SuccessfulTitle", "The operation finished successfully!"), Title);
	}
	else
	{
		OnMeshOperationError();
	}


	if (MaterialData != nullptr)
	{
		GetInstaLODInterface()->GetInstaLOD()->DeallocMaterialData(MaterialData);
		MaterialData = nullptr;
	}
	if (InputMesh != nullptr)
	{
		GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(InputMesh);
		InputMesh = nullptr;
	}
	if (OutputMesh != nullptr)
	{
		GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(OutputMesh);
		OutputMesh = nullptr;
	}
	if (Skeleton != nullptr)
	{
		GetInstaLODInterface()->GetInstaLOD()->DeallocSkeleton(Skeleton);
		Skeleton = nullptr;
	}
	if (BakeMesh != nullptr)
	{
		GetInstaLODInterface()->GetInstaLOD()->DeallocMesh(BakeMesh);
		BakeMesh = nullptr;
	}

	// NOTE: dealloc mesh operation last!
	DeallocMeshOperation();

	UEBoneIndexToInstaLODBoneIndexAndName.Empty();
}

bool UInstaLODBaseTool::ReadSettingsFromJSONObject(const TSharedPtr<FJsonObject>& JsonObject)
{
	return false;
}

FText UInstaLODBaseTool::GetFriendlyName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "BaseToolFriendlyName", "Base Tool");
}

FText UInstaLODBaseTool::GetToolBarToolTip() const
{
	return FText();
}

FText UInstaLODBaseTool::GetComboBoxItemName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "BaseToolComboBoxItemName", "Base Tool Item Name");
}

FText UInstaLODBaseTool::GetOperationInformation() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "BaseToolOperationInformation", "Base Tool Operation Information");
}

int32 UInstaLODBaseTool::GetOrderId() const
{
	return 0;
}

void UInstaLODBaseTool::ResetSettings()
{
	TargetLODIndex = 0;
	bReplaceSelectedMeshes = false;
	PivotPosition = EInstaLODPivotPosition::InstaLOD_Default;

	// Save the Config
	SaveConfig();
}

EInstaLODImportance UInstaLODBaseTool::GetImportanceValueForString(const FString& Value)
{
	if (Value.Equals("Off", ESearchCase::IgnoreCase))
		return EInstaLODImportance::InstaLOD_OFF;
	else if (Value.Equals("Lowest", ESearchCase::IgnoreCase))
		return EInstaLODImportance::InstaLOD_Lowest;
	else if (Value.Equals("Low", ESearchCase::IgnoreCase))
		return EInstaLODImportance::InstaLOD_Low;
	else if (Value.Equals("Normal", ESearchCase::IgnoreCase))
		return EInstaLODImportance::InstaLOD_Normal;
	else if (Value.Equals("High", ESearchCase::IgnoreCase))
		return EInstaLODImportance::InstaLOD_High;
	else if (Value.Equals("Highest", ESearchCase::IgnoreCase))
		return EInstaLODImportance::InstaLOD_Highest;

	UE_LOG(LogInstaLOD, Warning, TEXT("Type '%s' not supported for key '%s'"), *Value, TEXT("Importance"));
	return EInstaLODImportance::InstaLOD_Normal;
}

EInstaLODUnwrapStrategy UInstaLODBaseTool::GetUnwrapStrategyValueForString(const FString& Value)
{
	if (Value.Equals(TEXT("Auto"), ESearchCase::IgnoreCase))
		return EInstaLODUnwrapStrategy::InstaLOD_Auto;
	else if (Value.Equals(TEXT("Organic"), ESearchCase::IgnoreCase))
		return EInstaLODUnwrapStrategy::InstaLOD_Organic;
	else if (Value.Equals(TEXT("HardSurfaceAngle"), ESearchCase::IgnoreCase))
		return EInstaLODUnwrapStrategy::InstaLOD_HardSurfaceAngle;
	else if (Value.Equals(TEXT("HardSurfaceAxial"), ESearchCase::IgnoreCase))
		return EInstaLODUnwrapStrategy::InstaLOD_HardSurfaceAxial;

	UE_LOG(LogInstaLOD, Warning, TEXT("Type '%s' not supported for key '%s'"), *Value, TEXT("UnwrapStrategy"));
	return EInstaLODUnwrapStrategy::InstaLOD_Auto;
}

bool UInstaLODBaseTool::IsValidJSONObject(const TSharedPtr<FJsonObject>& JsonObject, const FString& MeshOperationType)
{
	if (!JsonObject.IsValid())
		return false;

	if (!JsonObject->HasField(TEXT("Settings")) || !JsonObject->HasField(TEXT("Type")))
		return false;

	const FString Type = JsonObject->GetStringField(TEXT("Type"));

	if (!Type.Equals(MeshOperationType, ESearchCase::IgnoreCase))
		return false;

	return true;
}

void UInstaLODBaseTool::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	const FProperty* const OutputPathProperty = UInstaLODBaseTool::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_STRING_CHECKED(UInstaLODBaseTool, NewAssetsOutputDirectory));
	check(OutputPathProperty != nullptr);

	if (!NewAssetsOutputDirectory.Path.IsEmpty() && PropertyChangedEvent.MemberProperty == OutputPathProperty)
	{
		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
		{
			static const FText ErrorMessageTitle = LOCTEXT("InstaLODFailedToSetOutputPathMessageTitle", "InstaLOD: Failed to set output path");

			FString PackageLocalPath;
			const FString ValidPackagePathFormat = FString::Format(TEXT("/Game/{0}"), { NewAssetsOutputDirectory.Path });
			const FString ContentDirectory = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
			if (!FPackageName::TryConvertGameRelativePackagePathToLocalPath(NewAssetsOutputDirectory.Path, PackageLocalPath) || !FPaths::IsUnderDirectory(PackageLocalPath, ContentDirectory))
			{
				FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, LOCTEXT("InstaLODInvalidParentDirectory", "The selected output path is invalid.\nPlease select a path in the project content directory."), ErrorMessageTitle);
				NewAssetsOutputDirectory.Path.Reset();
			}
			else if (!FPackageName::IsValidTextForLongPackageName(ValidPackagePathFormat))
			{
				FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, LOCTEXT("InstaLODInvalidFolderName", "The selected output path is invalid.\nPlease remove any special characters or spaces in the path and try again."), ErrorMessageTitle);
				NewAssetsOutputDirectory.Path.Reset();
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

template<typename T>
FString UInstaLODBaseTool::GetAssetOutputPathForType(const FString& SuggestedName) const
{
	FString OutputPath;

	if (NewAssetsOutputDirectory.Path.IsEmpty() || ResultUsage != EInstaLODResultUsage::InstaLOD_NewAsset)
	{
		FSaveAssetDialogConfig SaveAssetDialogConfig;
		SaveAssetDialogConfig.DialogTitleOverride = FText::Format(NSLOCTEXT(LOCTEXT_NAMESPACE, "SaveNewAsset", "Save New {0}"), T::StaticClass()->GetDisplayNameText());
		SaveAssetDialogConfig.DefaultPath = FString();
		SaveAssetDialogConfig.DefaultAssetName = SuggestedName;
		SaveAssetDialogConfig.AssetClassNames = { T::StaticClass()->GetClassPathName() };
		SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::Disallow;

		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		OutputPath = FPackageName::ObjectPathToPackageName(ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig));
	}
	else
	{
		// NOTE: UE requires a path to start with a prefix folder when creating assets. "/Game/" is the equivalent of the Contents folder.
		const FString& OutputDirectory = FPaths::Combine(TEXT("/Game/"), NewAssetsOutputDirectory.Path);
		uint32 ExistingNameIndex = 0;

		do
		{
			const FString AssetName = ExistingNameIndex == 0u ? SuggestedName : FString::Printf(TEXT("%s_%u"), *SuggestedName, ExistingNameIndex);
			OutputPath = FPaths::Combine(OutputDirectory, AssetName);

			ExistingNameIndex++;

			if (ExistingNameIndex > 100u)
			{
				// If failing to detect if asset already exists, create a random Guid.
				OutputPath = FPaths::Combine(OutputDirectory, FString::Printf(TEXT("%s_%s"), *SuggestedName, *FGuid::NewGuid().ToString()));
				break;
			}

		} while (UInstaLODUtilities::CheckIfAssetExists(OutputPath));
	}

	return OutputPath;
}

/************************************************************************/
/* UObject Interface													*/
/************************************************************************/

UWorld* UInstaLODBaseTool::GetWorld() const
{
	return GEditor->GetEditorWorldContext().World();
}

#undef LOCTEXT_NAMESPACE
