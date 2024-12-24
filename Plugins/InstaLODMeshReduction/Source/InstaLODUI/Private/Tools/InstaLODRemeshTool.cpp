/**
 * InstaLODRemeshTool.cpp (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODRemeshTool.cpp
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#include "InstaLODRemeshTool.h"
#include "InstaLODUIPCH.h"

#include "RawMesh.h"
#include "IContentBrowserSingleton.h"
#include "Interfaces/IPluginManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "RawMesh.h"

#include "Utilities/InstaLODUtilities.h"
#include "Slate/InstaLODWindow.h"

#define LOCTEXT_NAMESPACE "InstaLODUI"

UInstaLODRemeshTool::UInstaLODRemeshTool() : Super(),
Operation(nullptr),
OperationResult()
{
	// initialize state
	SetActiveSetting(SettingsCheckBox, false);
}

void UInstaLODRemeshTool::SetActiveSetting(EInstaLODRemeshType NewType, bool bForceSave)
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

void UInstaLODRemeshTool::OnMeshOperationExecute(bool bIsAsynchronous)
{
	// --------------------------
	// can be run on child thread
	// --------------------------
	check(Operation == nullptr);

	static FScopedSlowTask* TaskProgress = nullptr;
	TaskProgress = SlowTaskProgress;
	static float LastProgress;
	LastProgress = 0.0f;
	InstaLOD::pfnRemeshProgressCallback ProgressCallback  = [](class InstaLOD::IRemeshingOperation *, InstaLOD::IInstaLODMesh*, const float ProgressInPercent)
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

	InstaLOD::RemeshingSettings RemeshingSettings = GetRemeshingSettings();

	if (BakeMesh != nullptr)
	{
		BakeMesh->AppendMesh(InputMesh);
		RemeshingSettings.BakeMesh = BakeMesh;
	}

	// alloc mesh operation
	Operation = GetInstaLODInterface()->GetInstaLOD()->AllocRemeshingOperation();
	Operation->SetProgressCallback(ProgressCallback);
	Operation->SetMaterialData(MaterialData);
	Operation->AddMesh(InputMesh);

	// update reduction selection
	SetActiveSetting(GetActiveSetting(), false);

	// execute
	OperationResult = Operation->Execute(OutputMesh, RemeshingSettings);
}

bool UInstaLODRemeshTool::IsMeshOperationSuccessful() const
{
	return Operation != nullptr && OperationResult.Success;
}

void UInstaLODRemeshTool::DeallocMeshOperation()
{
	check(Operation);
	GetInstaLODInterface()->GetInstaLOD()->DeallocRemeshingOperation(Operation);
	Operation = nullptr;
}

InstaLOD::IInstaLODMaterial* UInstaLODRemeshTool::GetBakeMaterial()
{
	if (!IsMeshOperationSuccessful())
		return nullptr;

	return OperationResult.BakeMaterial;
}

FText UInstaLODRemeshTool::GetFriendlyName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "RemeshToolFriendlyName", "RE");
}

FText UInstaLODRemeshTool::GetComboBoxItemName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "RemeshToolComboBoxItemName", "Remesh");
}

FText UInstaLODRemeshTool::GetOperationInformation() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "RemeshToolOperationInformation", "The Remesh operation reconstructs the mesh from scratch. In the process, it reduces the topological complexity of the mesh while preserving the visual outcome.\n\nUV coordinates are automatically created and the original mesh appearance is transferred onto the reconstructed mesh using InstaLOD's texture baker.");
}

int32 UInstaLODRemeshTool::GetOrderId() const
{
	return 2;
}

void UInstaLODRemeshTool::ResetSettings()
{
	RemeshMode = EInstaLODRemeshMode::InstaLOD_Reconstruct;
	FuzzyFaceCountTarget = EInstaLODRemeshFaceCount::InstaLOD_Normal;
	MaximumTriangles = 0;
	ScreenSizeInPixels = 300;
	PixelMergeDistance = 2;
	bAutomaticTextureSize = false;
	
	RemeshResolution = EInstaLODRemeshResolution::InstaLOD_Normal;
	bIgnoreBackfaces = true;
	HardAngleThreshold = 70.0f;
	WeldingDistance = 0.0f;
	GutterSizeInPixels = 5;
	UnwrapStrategy = EInstaLODUnwrapStrategy::InstaLOD_Auto;
	UnwrapStretchImportance = EInstaLODImportance::InstaLOD_Normal;
	bPostProcessLayout = true;
	bShellStitching = true;
	bInsertNormalSplits = false;
	bLockBoundaries = false;
	bDeterministic = false;
	SetActiveSetting(EInstaLODRemeshType::InstaLOD_FuzzyFaceCountTarget, false);
	bAverageNormalsAsRayDirection = false;
	bBakeIgnoreBackface = true;
	AutoRayLengthFactor = 1.0f; 
	BakeMeshSuffix = TEXT("_bake");

	// Reset Parent which ultimately ends in a SaveConfig() call to reset everything
	Super::ResetSettings();
}

InstaLOD::RemeshingSettings UInstaLODRemeshTool::GetRemeshingSettings()
{
	InstaLOD::RemeshingSettings Settings;

	Settings.SurfaceMode = (InstaLOD::RemeshSurfaceMode::Type)RemeshMode;

	switch (SettingsCheckBox)
	{
	case EInstaLODRemeshType::InstaLOD_FuzzyFaceCountTarget:
		Settings.FaceCountTarget = (InstaLOD::RemeshFaceCountTarget::Type)FuzzyFaceCountTarget;
		break;
	case EInstaLODRemeshType::InstaLOD_MaximumTriangles:
		Settings.MaximumTriangles = MaximumTriangles;
		break;
	case EInstaLODRemeshType::InstaLOD_ScreenSizeInPixels:
		Settings.ScreenSizeInPixels = ScreenSizeInPixels;
		Settings.ScreenSizePixelMergeDistance = PixelMergeDistance;
		Settings.ScreenSizeInPixelsAutomaticTextureSize = bAutomaticTextureSize;
		break;
	}

	Settings.BakeEngine = InstaLOD::BakeEngine::CPU;

	// Surface Construction
	switch (RemeshResolution)
	{
		case EInstaLODRemeshResolution::InstaLOD_Lowest:
			Settings.Resolution = 100;
			break;
		case EInstaLODRemeshResolution::InstaLOD_Low:
			Settings.Resolution = 150;
			break;
		case EInstaLODRemeshResolution::InstaLOD_Normal:
			Settings.Resolution = 256;
			break;
		case EInstaLODRemeshResolution::InstaLOD_High:
			Settings.Resolution = 512;
			break;
		case EInstaLODRemeshResolution::InstaLOD_VeryHigh:
			Settings.Resolution = 1024;
			break;
		case EInstaLODRemeshResolution::InstaLOD_Extreme:
			Settings.Resolution = 2048;
			break;
	}

	Settings.SurfaceConstructionIgnoreBackface = bIgnoreBackfaces;
	Settings.HardAngleThreshold = HardAngleThreshold;
	Settings.WeldDistance = WeldingDistance;
	Settings.GutterSizeInPixels = GutterSizeInPixels;
	Settings.UnwrapStrategy = (InstaLOD::UnwrapStrategy::Type)UnwrapStrategy;
	Settings.StretchImportance = (InstaLOD::MeshFeatureImportance::Type)UnwrapStretchImportance;
	Settings.SurfaceModeOptimizeLockBoundaries = bLockBoundaries;
	
	Settings.BakeAverageNormalsAsRayDirections = bAverageNormalsAsRayDirection;
	Settings.BakeAutomaticRayLengthFactor = AutoRayLengthFactor;
	Settings.IgnoreBackface = bBakeIgnoreBackface;

	Settings.PostProcessLayout = bPostProcessLayout;
	Settings.ShellStitching = bShellStitching;
	Settings.InsertNormalSplits = bInsertNormalSplits;
	Settings.AlphaMaskThreshold = AlphaMaskThreshold;
	Settings.Deterministic = bDeterministic;
	Settings.BakeOutput = GetBakeOutputSettings();

	return Settings;
}

bool UInstaLODRemeshTool::ReadSettingsFromJSONObject(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!UInstaLODBaseTool::IsValidJSONObject(JsonObject, TEXT("Remesh")))
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

	if (SettingsObject->HasField(TEXT("SurfaceMode")))
	{
		const FString SurfaceModeValue = SettingsObject->GetStringField(TEXT("SurfaceMode"));

		if (SurfaceModeValue.Equals(TEXT("Reconstruct"), ESearchCase::IgnoreCase))
		{
			RemeshMode = EInstaLODRemeshMode::InstaLOD_Reconstruct;
		}
		else if (SurfaceModeValue.Equals(TEXT("Optimize"), ESearchCase::IgnoreCase))
		{
			RemeshMode = EInstaLODRemeshMode::InstaLOD_Optimize;
		}
		else if (SurfaceModeValue.Equals(TEXT("ConvexHull"), ESearchCase::IgnoreCase))
		{
			RemeshMode = EInstaLODRemeshMode::InstaLOD_ConvexHull;
		}
		else if (SurfaceModeValue.Equals(TEXT("UV"), ESearchCase::IgnoreCase))
		{
			RemeshMode = EInstaLODRemeshMode::InstaLOD_UV;
		}
		else
		{
			UE_LOG(LogInstaLOD, Warning, TEXT("Type '%s' not supported for key '%s'"), *SurfaceModeValue, TEXT("SurfaceMode"));
		}
	}
	if (SettingsObject->HasField(TEXT("MaximumTriangles")))
	{
		MaximumTriangles = SettingsObject->GetIntegerField(TEXT("MaximumTriangles"));
	}
	if (SettingsObject->HasField(TEXT("ScreenSizeInPixels")))
	{
		ScreenSizeInPixels = SettingsObject->GetIntegerField(TEXT("ScreenSizeInPixels"));
	}
	if (SettingsObject->HasField(TEXT("ScreenSizePixelMergeDistance")))
	{
		PixelMergeDistance = SettingsObject->GetIntegerField(TEXT("ScreenSizePixelMergeDistance"));
	}
	if (SettingsObject->HasField(TEXT("GutterSizeInPixels")))
	{
		GutterSizeInPixels = SettingsObject->GetIntegerField(TEXT("GutterSizeInPixels"));
	}
	if (SettingsObject->HasField(TEXT("FuzzyFaceCountTarget")))
	{
		const FString FuzzyFaceCountTargetValue = SettingsObject->GetStringField(TEXT("FuzzyFaceCountTarget"));
		
		if (FuzzyFaceCountTargetValue.Equals(TEXT("Lowest"), ESearchCase::IgnoreCase))
		{
			FuzzyFaceCountTarget = EInstaLODRemeshFaceCount::InstaLOD_Lowest;
		}
		else if (FuzzyFaceCountTargetValue.Equals(TEXT("Low"), ESearchCase::IgnoreCase))
		{
			FuzzyFaceCountTarget = EInstaLODRemeshFaceCount::InstaLOD_Low;
		}
		else if (FuzzyFaceCountTargetValue.Equals(TEXT("Normal"), ESearchCase::IgnoreCase))
		{
			FuzzyFaceCountTarget = EInstaLODRemeshFaceCount::InstaLOD_Normal;
		}
		else if (FuzzyFaceCountTargetValue.Equals(TEXT("High"), ESearchCase::IgnoreCase))
		{
			FuzzyFaceCountTarget = EInstaLODRemeshFaceCount::InstaLOD_High;
		}
		else if (FuzzyFaceCountTargetValue.Equals(TEXT("Highest"), ESearchCase::IgnoreCase))
		{
			FuzzyFaceCountTarget = EInstaLODRemeshFaceCount::InstaLOD_Highest;
		}
		else
		{
			UE_LOG(LogInstaLOD, Warning, TEXT("Type '%s' not supported for key '%s'"), *FuzzyFaceCountTargetValue, TEXT("FuzzyFaceCountTarget"));
		}
	}
	if (SettingsObject->HasField(TEXT("IgnoreBackface")))
	{
		bIgnoreBackfaces = SettingsObject->GetBoolField(TEXT("IgnoreBackface"));
	}
	if (SettingsObject->HasField(TEXT("StretchImportance")))
	{
		const FString Importance = SettingsObject->GetStringField(TEXT("StretchImportance"));
		UnwrapStretchImportance = UInstaLODBaseTool::GetImportanceValueForString(Importance);
	}
	if (SettingsObject->HasField(TEXT("UnwrapStrategy")))
	{
		const FString UnwrapStrategyValue = SettingsObject->GetStringField(TEXT("UnwrapStrategy"));
		UnwrapStrategy = UInstaLODBaseTool::GetUnwrapStrategyValueForString(UnwrapStrategyValue);
	}
	if (SettingsObject->HasField(TEXT("AlphaMaskThreshold")))
	{
		AlphaMaskThreshold = SettingsObject->GetNumberField(TEXT("AlphaMaskThreshold"));
	}
	if (SettingsObject->HasField(TEXT("ShellStitching")))
	{
		bShellStitching = SettingsObject->GetBoolField(TEXT("ShellStitching"));
	}
	if (SettingsObject->HasField(TEXT("InsertNormalSplits")))
	{
		bInsertNormalSplits = SettingsObject->GetBoolField(TEXT("InsertNormalSplits"));
	}
	if (SettingsObject->HasField(TEXT("PostProcessLayout")))
	{
		bPostProcessLayout = SettingsObject->GetBoolField(TEXT("PostProcessLayout"));
	}
	if (SettingsObject->HasField(TEXT("SurfaceModeOptimizeLockBoundaries")))
	{
		bLockBoundaries = SettingsObject->GetBoolField(TEXT("SurfaceModeOptimizeLockBoundaries"));
	}
	if (SettingsObject->HasField(TEXT("Deterministic")))
	{
		bDeterministic = SettingsObject->GetBoolField(TEXT("Deterministic"));
	}
	if (SettingsObject->HasField(TEXT("IgnoreBackface")))
	{
		bBakeIgnoreBackface = SettingsObject->GetBoolField(TEXT("IgnoreBackface"));
	}
	if (SettingsObject->HasField(TEXT("BakeAutomaticRayLengthFactor")))
	{
		AutoRayLengthFactor = SettingsObject->GetNumberField(TEXT("BakeAutomaticRayLengthFactor"));
	}
	if (SettingsObject->HasField(TEXT("BakeAverageNormalsAsRayDirections")))
	{
		bAverageNormalsAsRayDirection = SettingsObject->GetBoolField(TEXT("BakeAverageNormalsAsRayDirections"));
	}

	UInstaLODBakeBaseTool::ReadSettingsFromJSONObject(SettingsObject);

	return true;
}
#undef LOCTEXT_NAMESPACE
