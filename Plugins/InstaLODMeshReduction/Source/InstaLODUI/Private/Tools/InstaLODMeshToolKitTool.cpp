/**
 * InstaLODUVTool.cpp (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODUVTool.cpp 
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#include "InstaLODMeshToolKitTool.h"
#include "InstaLODUIPCH.h"

#include "Utilities/InstaLODUtilities.h"
#include "Slate/InstaLODWindow.h"

#define LOCTEXT_NAMESPACE "InstaLODUI"

UInstaLODMeshToolKitTool::UInstaLODMeshToolKitTool() : Super(),
Operation(nullptr),
OperationResult()
{
}

InstaLOD::MeshToolKitSettings UInstaLODMeshToolKitTool::GetMeshToolKitSettings()
{
	InstaLOD::MeshToolKitSettings settings;

	auto fnGetNormalHealingMode = [](const EInstaLODNormalHealing mode) -> InstaLOD::NormalHealingMode::Type
	{
		switch (mode)
		{
			case EInstaLODNormalHealing::InstaLOD_OFF :
				return InstaLOD::NormalHealingMode::Off;
			case EInstaLODNormalHealing::InstaLOD_Minimal:
				return InstaLOD::NormalHealingMode::Minimal;
			case EInstaLODNormalHealing::InstaLOD_Default:
				return InstaLOD::NormalHealingMode::Default;
		}
		return InstaLOD::NormalHealingMode::Off;
	};

	settings.WeldingThreshold = VertexWelding;
	settings.WeldingNormalAngleThreshold = VertexWeldingNormalAngle;
	settings.WeldingBoundaries = bWeldingBoundaries;
	settings.HealTJunctionThreshold = TJunctionHealing;
	settings.RemoveDegenerateFacesThreshold = RemoveDegenerateFaces;
	settings.FixNonManifold = bFixNonManifold;
	settings.ConformNormals = bConformNormals;
	settings.ConformWindingOrder = bConformWindingOrder;
	settings.FlipNormals = bFlipNormals;
	settings.FlipWindingOrder = bFlipWindingOrder;
	settings.FillHoles = bFillHoles;
	settings.NormalHealingMode = fnGetNormalHealingMode(NormalHealingMode);
	settings.RecalculateNormals = bRecalculateNormals;
	settings.HardAngleThreshold = HardangleThreshold;
	settings.WeightedNormals = bWeightedNormals;
	settings.MinimumSubMeshBoundingSphereRadius = MinimumRadius;
	settings.MinimumFaceArea = MinimumFaceArea;
	settings.Deterministic = bDeterministic;
	return settings;
}

void UInstaLODMeshToolKitTool::OnMeshOperationExecute(bool bIsAsynchronous)
{
	// --------------------------
	// can be run on child thread
	// --------------------------
	check(Operation == nullptr);

	static FScopedSlowTask* TaskProgress = nullptr;
	TaskProgress = SlowTaskProgress;
	static float LastProgress;
	LastProgress = 0.0f;
	InstaLOD::pfnMeshToolKitProgressCallback ProgressCallback = [](InstaLOD::IMeshToolKitOperation* MeshToolKitOperation, const InstaLOD::IInstaLODMesh* SourceMesh, InstaLOD::IInstaLODMesh* TargetMesh, const float ProgressInPercent)
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
	Operation = GetInstaLODInterface()->GetInstaLOD()->AllocMeshToolKitOperation();
	Operation->SetProgressCallback(ProgressCallback); 

	// execute
	OperationResult = Operation->Execute(InputMesh, OutputMesh, GetMeshToolKitSettings());
}

void UInstaLODMeshToolKitTool::ResetSettings()
{
	VertexWelding = 0.0f;
	VertexWeldingNormalAngle = 80.0f;
	TJunctionHealing = 0.0f;
	RemoveDegenerateFaces = 0.0f;
	bFixNonManifold = false;
	bConformNormals = false;
	bConformWindingOrder = false;
	bFlipNormals = false;
	bFlipWindingOrder = false;
	bFillHoles = false;
	NormalHealingMode = EInstaLODNormalHealing::InstaLOD_OFF;
	bRecalculateNormals = false;
	HardangleThreshold = 80.0f;
	bWeightedNormals = true;
	MinimumRadius = 0.0f;
	MinimumFaceArea = 0.0f;
	bDeterministic = false;
}

bool UInstaLODMeshToolKitTool::ReadSettingsFromJSONObject(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!UInstaLODBaseTool::IsValidJSONObject(JsonObject, TEXT("MeshToolKit")))
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

	if (SettingsObject->HasField(TEXT("MinimumFaceArea")))
	{
		MinimumFaceArea = (float)SettingsObject->GetNumberField(TEXT("MinimumFaceArea"));
	}
	if (SettingsObject->HasField(TEXT("MinimumSubMeshBoundingSphereRadius")))
	{
		MinimumRadius = (float)SettingsObject->GetNumberField(TEXT("MinimumSubMeshBoundingSphereRadius"));
	}
	if (SettingsObject->HasField(TEXT("WeldingThreshold")))
	{
		VertexWelding = (float)SettingsObject->GetNumberField(TEXT("WeldingThreshold"));
	}
	if (SettingsObject->HasField(TEXT("WeldingNormalAngleThreshold")))
	{
		VertexWeldingNormalAngle = (float)SettingsObject->GetNumberField(TEXT("WeldingNormalAngleThreshold"));
	}
	if (SettingsObject->HasField(TEXT("WeldingBoundaries")))
	{
		bWeldingBoundaries = SettingsObject->GetBoolField(TEXT("WeldingBoundaries"));
	}
	if (SettingsObject->HasField(TEXT("HealTJunctionThreshold")))
	{
		TJunctionHealing = (float)SettingsObject->GetNumberField(TEXT("HealTJunctionThreshold"));
	}
	if (SettingsObject->HasField(TEXT("RemoveDegenerateFacesThreshold")))
	{
		RemoveDegenerateFaces = (float)SettingsObject->GetNumberField(TEXT("RemoveDegenerateFacesThreshold"));
	}
	if (SettingsObject->HasField(TEXT("FixNonManifold")))
	{
		bFixNonManifold = SettingsObject->GetBoolField(TEXT("FixNonManifold"));
	}
	if (SettingsObject->HasField(TEXT("FillHoles")))
	{
		bFillHoles = SettingsObject->GetBoolField(TEXT("FillHoles"));
	}
	if (SettingsObject->HasField(TEXT("ConformNormals")))
	{
		bConformNormals = SettingsObject->GetBoolField(TEXT("ConformNormals"));
	}
	if (SettingsObject->HasField(TEXT("FlipNormals")))
	{
		bFlipNormals = SettingsObject->GetBoolField(TEXT("FlipNormals"));
	}
	if (SettingsObject->HasField(TEXT("FlipWindingOrder")))
	{
		bFlipWindingOrder = SettingsObject->GetBoolField(TEXT("FlipWindingOrder"));
	}
	if (SettingsObject->HasField(TEXT("RecalculateNormals")))
	{
		bRecalculateNormals = SettingsObject->GetBoolField(TEXT("RecalculateNormals"));
	}
	if (SettingsObject->HasField(TEXT("WeightedNormals")))
	{
		bWeightedNormals = SettingsObject->GetBoolField(TEXT("WeightedNormals"));
	}
	if (SettingsObject->HasField(TEXT("HardAngleThreshold")))
	{
		HardangleThreshold = (float)SettingsObject->GetNumberField(TEXT("HardAngleThreshold"));
	}
	if (SettingsObject->HasField(TEXT("NormalHealingMode")))
	{
		const FString NormalHealingModeValue = SettingsObject->GetStringField(TEXT("NormalHealingMode"));

		if (NormalHealingModeValue.Equals(TEXT("Off"), ESearchCase::IgnoreCase))
		{
			NormalHealingMode = EInstaLODNormalHealing::InstaLOD_OFF;
		}
		else if (NormalHealingModeValue.Equals(TEXT("Minimal"), ESearchCase::IgnoreCase))
		{
			NormalHealingMode = EInstaLODNormalHealing::InstaLOD_Minimal;
		}
		else if (NormalHealingModeValue.Equals(TEXT("Default"), ESearchCase::IgnoreCase))
		{
			NormalHealingMode = EInstaLODNormalHealing::InstaLOD_Default;
		}
		else
		{
			UE_LOG(LogInstaLOD, Warning, TEXT("Type '%s' not supported for key '%s'"), *NormalHealingModeValue, TEXT("NormalHealingMode"));
		}
	}
	if (SettingsObject->HasField(TEXT("Deterministic")))
	{
		bDeterministic = SettingsObject->GetBoolField(TEXT("Deterministic"));
	}

	return true;
}

void UInstaLODMeshToolKitTool::DeallocMeshOperation() 
{
	if (Operation != nullptr)
	{
		GetInstaLODInterface()->GetInstaLOD()->DeallocMeshToolKitOperation(Operation);
		Operation = nullptr;
	}
}

bool UInstaLODMeshToolKitTool::IsMeshOperationSuccessful() const
{
	return Operation != nullptr && OperationResult.Success;
};

FText UInstaLODMeshToolKitTool::GetFriendlyName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "MTKToolFriendlyName", "MTK");
}

FText UInstaLODMeshToolKitTool::GetComboBoxItemName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "MTKToolComboBoxItemName", "Mesh Toolkit");
}

FText UInstaLODMeshToolKitTool::GetOperationInformation() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "MTKToolOperationInformation", "The Mesh Toolkit operation is a swiss-army knife for geometry processing. From welding vertices to conforming normals to point into the same direction or creating a convex hull for input geometry - the list of tools is exhaustive.");
}

int32 UInstaLODMeshToolKitTool::GetOrderId() const
{
	return 7;
}
