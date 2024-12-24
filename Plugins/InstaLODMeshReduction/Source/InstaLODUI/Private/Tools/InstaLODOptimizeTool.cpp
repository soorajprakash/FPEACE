/**
 * InstaLODOptimizeTool.cpp (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODOptimizeTool.cpp
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#include "InstaLODOptimizeTool.h"
#include "InstaLODUIPCH.h"
#include "InstaLODModule.h"

#include "RawMesh.h"
#include "IContentBrowserSingleton.h"
#include "Interfaces/IPluginManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"

#include "Utilities/InstaLODUtilities.h"

#define LOCTEXT_NAMESPACE "InstaLODUI"

UInstaLODOptimizeTool::UInstaLODOptimizeTool() : Super(),
BakePose(nullptr),
Operation(nullptr),
OperationResult()
{
	// initialize state
	SetActiveSetting(SettingsCheckBox, false);
}

void UInstaLODOptimizeTool::OnNewSelection()
{
	Super::OnNewSelection();
	BakePose = nullptr;
}

void UInstaLODOptimizeTool::SetActiveSetting(EInstaLODOptimizeType NewType, bool bForceSave)
{
	if (SettingsCheckBox != NewType)
	{
		SettingsCheckBox = NewType;
	}

	if (bForceSave)
	{
		SaveConfig();
	}
}

FText UInstaLODOptimizeTool::GetFriendlyName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "OptimizeToolFriendlyName", "OP");
}

FText UInstaLODOptimizeTool::GetComboBoxItemName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "OptimizeToolComboBoxItemName", "Optimize");
}

FText UInstaLODOptimizeTool::GetOperationInformation() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "OptimizeToolOperationInformation", "The Optimize operation carefully removes polygons and relocates vertices until your polygon target has been met. Mesh attributes, such as normals or UV coordinates, are adjusted to preserve the appearence of the mesh.");
}

int32 UInstaLODOptimizeTool::GetOrderId() const
{
	return 1;
}

FSkeletalMeshOptimizationSettings UInstaLODOptimizeTool::GetSkeletalMeshOptimizationSettings(const int32 BaseLOD)
{
	FSkeletalMeshOptimizationSettings Settings;

	Settings.SkinningImportance = (SkeletalMeshOptimizationImportance)SkinningImportance;
	Settings.ShadingImportance = (SkeletalMeshOptimizationImportance)ShadingImportance;
	Settings.TextureImportance = (SkeletalMeshOptimizationImportance)TextureImportance;
	Settings.SilhouetteImportance = (SkeletalMeshOptimizationImportance)SilhouetteImportance;
	
#if defined(INSTALOD_SKELETON_OPTIMIZE)
	Settings.MaxBonesPerVertex = MaxInfluencePerVertex;
#endif
	
	Settings.NumOfTrianglesPercentage = PercentTriangles;
	Settings.NormalsThreshold = HardAngleThreshold;
	Settings.MaxDeviationPercentage = MaximumDeviation;
	Settings.WeldingThreshold = WeldingDistance;
	Settings.bRecalcNormals = bRecalculateNormals;
	Settings.BaseLOD = BaseLOD;

	return Settings;
}

void UInstaLODOptimizeTool::ResetSettings()
{
	SetActiveSetting(EInstaLODOptimizeType::InstaLOD_PercentTriangles, false);

	AutomaticQuality = EInstaLODImportance::InstaLOD_OFF;

	PercentTriangles = 50.0f;
	AbsoluteTriangles = 0;
	ScreenSizeInPixels = 300;
	MaximumDeviation = 0.0f;
	
	BoundaryImportance = EInstaLODImportance::InstaLOD_Normal;
	TextureImportance = EInstaLODImportance::InstaLOD_Normal;
	ShadingImportance = EInstaLODImportance::InstaLOD_Normal;
	SilhouetteImportance = EInstaLODImportance::InstaLOD_Normal;
	SkinningImportance = EInstaLODImportance::InstaLOD_Normal;
	
	bRecalculateNormals = false;
	HardAngleThreshold = 80.f;

	bProtectSplits = true;
	bProtectBoundaries = true;
	bOptimalPlacement = true;
	bNormalizeMeshScale = false;
	bDeterministic = false;
	
#if defined(INSTALOD_SKELETON_OPTIMIZE)
	BakePose = nullptr;
	LeafBoneWeldDistance = 0.0f;
	MaximumBoneDepth = 0;
	MaxInfluencePerVertex = 0;
	MinInfluenceThreshold = 0.0f;
	IgnoreJointRegEx = "";
#endif
	WeldingDistance = 0.0f;
	TJunctionHealingDistance = 0.0f;
	bLockSplits = false;
	bLockBoundaries = false;
	bVertexColorsAsOptimizerWeights = false;

	// Reset Parent which ultimately ends in a SaveConfig() call to reset everything
	Super::ResetSettings();
}

void UInstaLODOptimizeTool::OnMeshOperationExecute(bool bIsAsynchronous)
{
	// --------------------------
	// can be run on child thread
	// --------------------------
	check(Operation == nullptr);

	static FScopedSlowTask* TaskProgress = nullptr;
	TaskProgress = SlowTaskProgress;
	static float LastProgress;
	LastProgress = 0.0f;
	InstaLOD::pfnOptimizationProgressCallback ProgressCallback = [](InstaLOD::IOptimizeOperation *, const InstaLOD::IInstaLODMesh* , InstaLOD::IInstaLODMesh* ,
																	const float ProgressInPercent, const uint32, const uint32 )
		{
			if (FMath::IsNearlyEqual(LastProgress, ProgressInPercent, KINDA_SMALL_NUMBER) || ProgressInPercent < 0.0f)
				return;

			if (LastProgress >= 1.0f)
			{
				LastProgress = 0.0f;
				return;
			}

			const float DeltaProgress = FMath::Max(0.0f, (ProgressInPercent - LastProgress) * 100.0f);
			LastProgress = ProgressInPercent;

			if (!IsInGameThread())
			{
				AsyncTask(ENamedThreads::GameThread, [DeltaProgress]()
						  {
								TaskProgress->EnterProgressFrame(DeltaProgress);
						  });
			}
			else
			{
				TaskProgress->EnterProgressFrame(DeltaProgress);
			}
		};

	if (bVertexColorsAsOptimizerWeights)
	{
		const bool bDidCreateOptimizerWeights = InputMesh->ConvertColorDataToOptimizerWeights(0);
	
		if (!bDidCreateOptimizerWeights)
		{
			UE_LOG(LogInstaLOD, Warning, TEXT("Failed to convert vertex colors to optimizer weights."));
		}
	}
	
	// alloc mesh operation
	Operation = GetInstaLODInterface()->GetInstaLOD()->AllocOptimizeOperation();
	Operation->SetProgressCallback(ProgressCallback);

	InstaLOD::OptimizeSettings OptimizeSettings = GetOptimizeSettings();
	
	bool bIsSkeletalOptimizationEnabled = bSingleSkeletalMeshSelected && Skeleton != nullptr &&
	(OptimizeSettings.SkeletonOptimize.LeafBoneWeldDistance != 0 ||
	OptimizeSettings.SkeletonOptimize.MaximumBoneDepth != 0 ||
	OptimizeSettings.SkeletonOptimize.MinimumBoneInfluenceThreshold != 0 ||
	OptimizeSettings.SkeletonOptimize.MaximumBoneInfluencesPerVertex != 0);

	TArray<uint32> IgnoreJointIndices;

	if(bIsSkeletalOptimizationEnabled)
	{
		OptimizeSettings.Skeleton = Skeleton;

		if(!IgnoreJointRegEx.IsEmpty())
		{
			// find matching strings
			FRegexPattern RegexPattern(IgnoreJointRegEx);

			for (TPair<int32, TPair<uint32, FString>> Values : UEBoneIndexToInstaLODBoneIndexAndName)
			{
				FRegexMatcher Match(RegexPattern, Values.Value.Value);

				if(Match.FindNext())
					IgnoreJointIndices.Push(Values.Value.Key);
			}

			OptimizeSettings.SkeletonOptimize.IgnoreJointIndices = IgnoreJointIndices.GetData();
			OptimizeSettings.SkeletonOptimize.IgnoreJointIndicesCount = IgnoreJointIndices.Num();
		}
	}

	// execute
	OperationResult = Operation->Execute(InputMesh, OutputMesh, OptimizeSettings);
}

bool UInstaLODOptimizeTool::IsMeshOperationSuccessful() const
{
	return Operation != nullptr && OperationResult.Success;
}

void UInstaLODOptimizeTool::DeallocMeshOperation()
{
	check(Operation);
	GetInstaLODInterface()->GetInstaLOD()->DeallocOptimizeOperation(Operation);
	Operation = nullptr;
}

InstaLOD::OptimizeSettings UInstaLODOptimizeTool::GetOptimizeSettings()
{
	InstaLOD::OptimizeSettings ReturnSettings;

	ReturnSettings.AutomaticQuality = (InstaLOD::MeshFeatureImportance::Type)AutomaticQuality;

	if (ReturnSettings.AutomaticQuality == InstaLOD::MeshFeatureImportance::Off)
	{
		switch (SettingsCheckBox)
		{
		case EInstaLODOptimizeType::InstaLOD_PercentTriangles:
			ReturnSettings.PercentTriangles = PercentTriangles / 100;
			UE_LOG(LogTemp, Log, TEXT("## InstaLOD Optimize Settings | Used PercentTriangles ##"));
			break;
		case EInstaLODOptimizeType::InstaLOD_AbsoluteTriangles:
			ReturnSettings.AbsoluteTriangles = AbsoluteTriangles;
			UE_LOG(LogTemp, Log, TEXT("## InstaLOD Optimize Settings | Used AbsoluteTriangles ##"));
			break;
		case EInstaLODOptimizeType::InstaLOD_ScreenSizeInPixels:
			ReturnSettings.ScreenSizeInPixels = ScreenSizeInPixels;
			UE_LOG(LogTemp, Log, TEXT("## InstaLOD Optimize Settings | Used ScreenSizeInPixels ##"));
			break;
		}
	}

	ReturnSettings.MaxDeviation = MaximumDeviation;
	ReturnSettings.AlgorithmStrategy = InstaLOD::AlgorithmStrategy::Smart_v2;

	// TODO: Wrap with function to ignore order problems
	ReturnSettings.TextureImportance = (InstaLOD::MeshFeatureImportance::Type)TextureImportance;
	ReturnSettings.ShadingImportance = (InstaLOD::MeshFeatureImportance::Type)ShadingImportance;
	ReturnSettings.SkinningImportance = (InstaLOD::MeshFeatureImportance::Type)SkinningImportance;
	ReturnSettings.SilhouetteImportance = (InstaLOD::MeshFeatureImportance::Type)SilhouetteImportance;

	ReturnSettings.RecalculateNormals = bRecalculateNormals;
	ReturnSettings.HardAngleThreshold = HardAngleThreshold;

#if defined(INSTALOD_SKELETON_OPTIMIZE)
	ReturnSettings.SkeletonOptimize.LeafBoneWeldDistance = LeafBoneWeldDistance;
	ReturnSettings.SkeletonOptimize.MaximumBoneDepth = MaximumBoneDepth;
	ReturnSettings.SkeletonOptimize.MaximumBoneInfluencesPerVertex = MaxInfluencePerVertex;
	ReturnSettings.SkeletonOptimize.MinimumBoneInfluenceThreshold = MinInfluenceThreshold;

	// 
	//ReturnSettings.SkeletonOptimize.IgnoreJointIndices = 
#endif
	
	ReturnSettings.WeldingThreshold = WeldingDistance;
	ReturnSettings.HealTJunctionThreshold = TJunctionHealingDistance;

	ReturnSettings.LockSplits = bLockSplits;
	ReturnSettings.LockBoundaries = bLockBoundaries;
	ReturnSettings.ProtectSplits = bProtectSplits;
	ReturnSettings.ProtectBoundaries = bProtectBoundaries;
	ReturnSettings.OptimalPlacement = bOptimalPlacement;
	ReturnSettings.NormalizeMeshScale = bNormalizeMeshScale;
	ReturnSettings.OptimizerVertexWeights = bVertexColorsAsOptimizerWeights;
	ReturnSettings.Deterministic = bDeterministic;

	return ReturnSettings;
}

bool UInstaLODOptimizeTool::ReadSettingsFromJSONObject(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!UInstaLODBaseTool::IsValidJSONObject(JsonObject, TEXT("Optimize")))
		return false;

	const TSharedPtr<FJsonObject>* SettingsObjectPointer = nullptr;

	if (!JsonObject->TryGetObjectField(FString(TEXT("Settings")), SettingsObjectPointer) ||
		SettingsObjectPointer == nullptr ||
		!SettingsObjectPointer->IsValid())
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: Could not retrieve Settings field."));
		return false;
	}

	const TSharedPtr<FJsonObject>& SettingsObject = *SettingsObjectPointer;

	if (SettingsObject->HasField(TEXT("AutomaticQuality")))
	{
		const FString quality = SettingsObject->GetStringField(TEXT("AutomaticQuality"));
		AutomaticQuality = UInstaLODBaseTool::GetImportanceValueForString(quality);
	}
	if (SettingsObject->HasField(TEXT("AbsoluteTriangles")))
	{
		AbsoluteTriangles = SettingsObject->GetIntegerField(TEXT("AbsoluteTriangles"));
	}
	if (SettingsObject->HasField(TEXT("MaxDeviation")))
	{
		MaximumDeviation = SettingsObject->GetNumberField(TEXT("MaxDeviation"));
	}
	if (SettingsObject->HasField(TEXT("HardAngleThreshold")))
	{
		HardAngleThreshold = SettingsObject->GetNumberField(TEXT("HardAngleThreshold"));
	}
	if (SettingsObject->HasField(TEXT("PercentTriangles")))
	{
		PercentTriangles = (float)SettingsObject->GetNumberField(TEXT("PercentTriangles")) * 100.0f;
	}
	if (SettingsObject->HasField(TEXT("ScreenSizeInPixels")))
	{
		ScreenSizeInPixels = SettingsObject->GetIntegerField(TEXT("ScreenSizeInPixels"));
	}
	if (SettingsObject->HasField(TEXT("RecalculateNormals")))
	{
		bRecalculateNormals = SettingsObject->GetBoolField(TEXT("RecalculateNormals"));
	}
	if (SettingsObject->HasField(TEXT("LockSplits")))
	{
		bLockSplits = SettingsObject->GetBoolField(TEXT("LockSplits"));
	}
	if (SettingsObject->HasField(TEXT("LockBoundaries")))
	{
		bLockBoundaries = SettingsObject->GetBoolField(TEXT("LockBoundaries"));
	}
	if (SettingsObject->HasField(TEXT("ProtectSplits")))
	{
		bProtectSplits = SettingsObject->GetBoolField(TEXT("ProtectSplits"));
	}
	if (SettingsObject->HasField(TEXT("ProtectBoundaries")))
	{
		bProtectBoundaries = SettingsObject->GetBoolField(TEXT("ProtectBoundaries"));
	}
	if (SettingsObject->HasField(TEXT("OptimalPlacement")))
	{
		bOptimalPlacement = SettingsObject->GetBoolField(TEXT("OptimalPlacement"));
	}
	if (SettingsObject->HasField(TEXT("NormalizeMeshScale")))
	{
		bNormalizeMeshScale = SettingsObject->GetBoolField(TEXT("NormalizeMeshScale"));
	}
	if (SettingsObject->HasField(TEXT("HealTJunctionThreshold")))
	{
		TJunctionHealingDistance = SettingsObject->GetNumberField(TEXT("HealTJunctionThreshold"));
	}
	if (SettingsObject->HasField(TEXT("BoundaryImportance")))
	{
		const FString Importance = SettingsObject->GetStringField(TEXT("BoundaryImportance"));
		BoundaryImportance = UInstaLODBaseTool::GetImportanceValueForString(Importance);
	}
	if (SettingsObject->HasField(TEXT("ShadingImportance")))
	{
		const FString Importance = SettingsObject->GetStringField(TEXT("ShadingImportance"));
		ShadingImportance = UInstaLODBaseTool::GetImportanceValueForString(Importance);
	}
	if (SettingsObject->HasField(TEXT("SilhouetteImportance")))
	{
		const FString Importance = SettingsObject->GetStringField(TEXT("SilhouetteImportance"));
		SilhouetteImportance = UInstaLODBaseTool::GetImportanceValueForString(Importance);
	}
	if (SettingsObject->HasField(TEXT("SkinningImportance")))
	{
		const FString Importance = SettingsObject->GetStringField(TEXT("SkinningImportance"));
		SkinningImportance = UInstaLODBaseTool::GetImportanceValueForString(Importance);
	}
	if (SettingsObject->HasField(TEXT("TextureImportance")))
	{
		const FString Importance = SettingsObject->GetStringField(TEXT("TextureImportance"));
		TextureImportance = UInstaLODBaseTool::GetImportanceValueForString(Importance);
	}
	if (SettingsObject->HasField(TEXT("Deterministic")))
	{
		bDeterministic = SettingsObject->GetBoolField(TEXT("Deterministic"));
	}
	if (SettingsObject->HasField(TEXT("SkeletonOptimize")))
	{
		const TSharedPtr<FJsonObject>& SkeletonOptimize = SettingsObject->GetObjectField(TEXT("SkeletonOptimize"));

		if (SkeletonOptimize->HasField(TEXT("LeafBoneWeldDistance")))
		{
			LeafBoneWeldDistance = (float) SkeletonOptimize->GetNumberField(TEXT("LeafBoneWeldDistance"));
		}
		if (SkeletonOptimize->HasField(TEXT("MaximumBoneDepth")))
		{
			MaximumBoneDepth = SkeletonOptimize->GetIntegerField(TEXT("MaximumBoneDepth"));
		}
		if (SkeletonOptimize->HasField(TEXT("MaximumBoneInfluencesPerVertex")))
		{
			MaxInfluencePerVertex = SkeletonOptimize->GetIntegerField(TEXT("MaximumBoneInfluencesPerVertex"));
		}
		if (SkeletonOptimize->HasField(TEXT("MinimumBoneInfluenceThreshold")))
		{
			MinInfluenceThreshold = (float) SkeletonOptimize->GetNumberField(TEXT("MinimumBoneInfluenceThreshold"));
		}
		if (SkeletonOptimize->HasField(TEXT("IgnoreJointRegEx")))
		{
			IgnoreJointRegEx = SkeletonOptimize->GetStringField(TEXT("IgnoreJointRegEx"));
		}
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
