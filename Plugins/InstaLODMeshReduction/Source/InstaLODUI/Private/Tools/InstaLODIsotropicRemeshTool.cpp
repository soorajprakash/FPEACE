/**
 * InstaLODIsotropicRemeshTool.cpp (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODIsotropicRemeshTool.cpp
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#include "InstaLODIsotropicRemeshTool.h"
#include "InstaLODUIPCH.h"

#define LOCTEXT_NAMESPACE "InstaLODUI"

UInstaLODIsotropicRemeshTool::UInstaLODIsotropicRemeshTool() : Super(),
Operation(nullptr),
OperationResult()
{
}

void UInstaLODIsotropicRemeshTool::OnMeshOperationExecute(bool bIsAsynchronous)
{
	// --------------------------
	// can be run on child thread
	// --------------------------
	check(Operation == nullptr);

	static FScopedSlowTask* TaskProgress = nullptr;
	TaskProgress = SlowTaskProgress;
	static float LastProgress;
	LastProgress = 0.0f;
	InstaLOD::pfnIsoTropicRemeshingProgressCallback ProgressCallback = [](class InstaLOD::IIsotropicRemeshingOperation*, InstaLOD::IInstaLODMesh*, const float ProgressInPercent)
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

	// alloc mesh operation
	Operation = GetInstaLODInterface()->GetInstaLOD()->AllocIsotropicRemeshingOperation();
	Operation->SetProgressCallback(ProgressCallback);
	
	// execute
	OperationResult = Operation->Execute(InputMesh, OutputMesh, GetIsotropicRemeshingSettings());
}

bool UInstaLODIsotropicRemeshTool::IsMeshOperationSuccessful() const
{
	return Operation != nullptr && OperationResult.Success;
}

void UInstaLODIsotropicRemeshTool::DeallocMeshOperation()
{
	check(Operation);
	GetInstaLODInterface()->GetInstaLOD()->DeallocIsotropicRemeshingOperation(Operation);
	Operation = nullptr;
}

FText UInstaLODIsotropicRemeshTool::GetFriendlyName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "IsotropicRemeshToolFriendlyName", "ISO");
}

FText UInstaLODIsotropicRemeshTool::GetComboBoxItemName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "IsotropicRemeshToolComboBoxItemName", "Isotropic Remesh");
}

FText UInstaLODIsotropicRemeshTool::GetOperationInformation() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "IsotropicRemeshToolOperationInformation", "The Isotropic Remesh operation restructures the mesh topology to ensure that edges have consistent sizes. Mesh attributes, such as normals or UV coordinates , can be optionally preserved through the process.");
}

int32 UInstaLODIsotropicRemeshTool::GetOrderId() const
{
	return 8;
}

void UInstaLODIsotropicRemeshTool::ResetSettings()
{
	EdgeMode = EInstaLODIsotropicRemeshEdgeMode::InstaLOD_Automatic;
	TargetEdgeSize = 1.0f;
	Precision = EInstaLODImportance::InstaLOD_Normal;
	WeldingThreshold = 0.0f;
	GeometricFeatureAngleInDegrees = 0.0f;
	CollapseThreshold = 75.0f;
	bPreserveColorSplits = false;
	bPreserveUVSplits = true;
	bPreserveNormalSplits = false;
	bPreserveVolume = false;
	bRecalculateNormals = false;
	HardAngleThreshold = 80.0f;
	bWeightedNormals = true;
	bDeterministic = false;

	// Reset Parent which ultimately ends in a SaveConfig() call to reset everything
	Super::ResetSettings();
}

InstaLOD::IsotropicRemeshingSettings UInstaLODIsotropicRemeshTool::GetIsotropicRemeshingSettings()
{
	InstaLOD::IsotropicRemeshingSettings Settings;

	Settings.EdgeMode = (InstaLOD::IsotropicRemeshingEdgeMode::Type)EdgeMode;
	Settings.TargetEdgeSize = TargetEdgeSize;
	Settings.Precision = (InstaLOD::MeshFeatureImportance::Type)Precision;
	Settings.WeldingThreshold = WeldingThreshold;
	Settings.GeometricFeatureAngleInDegrees = GeometricFeatureAngleInDegrees;
	Settings.CollapseThreshold = CollapseThreshold / 100.0f;
	Settings.PreserveColorSplits = bPreserveColorSplits;
	Settings.PreserveTexCoordSplits = bPreserveUVSplits;
	Settings.PreserveNormalSplits = bPreserveNormalSplits;
	Settings.PreserveVolume = bPreserveVolume;
	Settings.RecalculateNormals = bRecalculateNormals;
	Settings.HardAngleThreshold = HardAngleThreshold;
	Settings.WeightedNormals = bWeightedNormals;
	Settings.Deterministic = bDeterministic;

	return Settings;
}

bool UInstaLODIsotropicRemeshTool::ReadSettingsFromJSONObject(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!UInstaLODBaseTool::IsValidJSONObject(JsonObject, TEXT("IsotropicRemesh")))
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

	if (SettingsObject->HasField(TEXT("EdgeMode")))
	{
		const FString EdgeModeValue = SettingsObject->GetStringField(TEXT("EdgeMode"));

		if (EdgeModeValue.Equals(TEXT("Automatic"), ESearchCase::IgnoreCase))
		{
			EdgeMode = EInstaLODIsotropicRemeshEdgeMode::InstaLOD_Automatic;
		}
		else if (EdgeModeValue.Equals(TEXT("Absolute"), ESearchCase::IgnoreCase))
		{
			EdgeMode = EInstaLODIsotropicRemeshEdgeMode::InstaLOD_Absolute;
		}
		else if (EdgeModeValue.Equals(TEXT("BoundingSphereRelative"), ESearchCase::IgnoreCase))
		{
			EdgeMode = EInstaLODIsotropicRemeshEdgeMode::InstaLOD_BoundingSphereRelative;
		}
		else
		{
			UE_LOG(LogInstaLOD, Warning, TEXT("Type '%s' not supported for key '%s'"), *EdgeModeValue, TEXT("EdgeMode"));
		}
	}
	if (SettingsObject->HasField(TEXT("TargetEdgeSize")))
	{
		TargetEdgeSize = SettingsObject->GetNumberField(TEXT("TargetEdgeSize"));
	}

	if (SettingsObject->HasField(TEXT("Precision")))
	{
		const FString Importance = SettingsObject->GetStringField(TEXT("Precision"));
		Precision = UInstaLODBaseTool::GetImportanceValueForString(Importance);
	}

	if (SettingsObject->HasField(TEXT("WeldingThreshold")))
	{
		WeldingThreshold = SettingsObject->GetNumberField(TEXT("WeldingThreshold"));
	}
	if (SettingsObject->HasField(TEXT("GeometricFeatureAngleInDegrees")))
	{
		GeometricFeatureAngleInDegrees = SettingsObject->GetIntegerField(TEXT("GeometricFeatureAngleInDegrees"));
	}
	if (SettingsObject->HasField(TEXT("CollapseThreshold")))
	{
		CollapseThreshold = SettingsObject->GetIntegerField(TEXT("CollapseThreshold"));
	}

	if (SettingsObject->HasField(TEXT("PreserveColorSplits")))
	{
		bPreserveColorSplits = SettingsObject->GetBoolField(TEXT("PreserveColorSplits"));
	}
	if (SettingsObject->HasField(TEXT("PreserveTexCoordSplits")))
	{
		bPreserveUVSplits = SettingsObject->GetBoolField(TEXT("PreserveTexCoordSplits"));
	}
	if (SettingsObject->HasField(TEXT("PreserveNormalSplits")))
	{
		bPreserveNormalSplits = SettingsObject->GetBoolField(TEXT("PreserveNormalSplits"));
	}
	if (SettingsObject->HasField(TEXT("PreserveVolume")))
	{
		bPreserveVolume = SettingsObject->GetBoolField(TEXT("PreserveVolume"));
	}

	if (SettingsObject->HasField(TEXT("RecalculateNormals")))
	{
		bRecalculateNormals = SettingsObject->GetBoolField(TEXT("RecalculateNormals"));
	}
	if (SettingsObject->HasField(TEXT("HardAngleThreshold")))
	{
		HardAngleThreshold = SettingsObject->GetNumberField(TEXT("HardAngleThreshold"));
	}
	if (SettingsObject->HasField(TEXT("PreserveVolume")))
	{
		bWeightedNormals = SettingsObject->GetBoolField(TEXT("PreserveVolume"));
	}
	if (SettingsObject->HasField(TEXT("Deterministic")))
	{
		bDeterministic = SettingsObject->GetBoolField(TEXT("Deterministic"));
	}
	return true;
}

#undef LOCTEXT_NAMESPACE