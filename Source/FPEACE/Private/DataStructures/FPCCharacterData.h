// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonStructs.h"
#include "Engine/DataAsset.h"
#include "FPCCharacterData.generated.h"

enum class ELocomotionStance : uint8;
class UFPCAnimInstance;
class AFPCWeapon;
enum class ELocomotionState : uint8;
enum class ECameraMode : uint8;

/**
 * 
 */
UCLASS()
class FPEACE_API UFPCCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECameraMode StartingCameraMode;

	UPROPERTY(EditDefaultsOnly)
	TMap<ECameraMode, FCharacterCameraModeSettings> CameraModeSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AFPCWeapon> DefaultWeaponBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=45, ClampMax=90))
	float ControllerRotationPitchClamp = 85.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> AnimClassTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.00001f, ClampMax = 0.1))
	float TurningLeanStrength = 0.05;

	UPROPERTY(EditDefaultsOnly)
	TMap<ELocomotionStance, FLocomotionStanceSetting> LocomotionStanceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCharacterDirectionAngleLimits CharacterDirectionLimits;
};
