// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/FPCActor.h"
#include "FPCWeapon.generated.h"

enum class ELocomotionState : uint8;
class AFPCCharacter;
class UFPCAnimInstance;
enum class ECameraMode : uint8;

USTRUCT(BlueprintType)
struct FWeaponAnimSettings
{
	GENERATED_BODY()

	/*
	 * This is the time taken to point the weapon towards aim direction.
	 * Basically like Aim Down Sight time for a gun
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun")
	float FocusTime = 1;

	/*
	 * This is the multiplier to determine how much the camera's FOV must be reduced for this weapon
	 */
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=0.1, ClampMax=1))
	float FirstPersonADSFieldOfViewMultiplier = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ELocomotionState, FTransform> DefaultLocomotionStateOffsets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ELocomotionState, FTransform> ADSLocomotionStateOffsets;
};

USTRUCT(BlueprintType)
struct FWeaponLagSettingSection
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Location")
	FVector HorizontalLocationLag;
	UPROPERTY(EditAnywhere, Category="Location")
	FVector VerticalLocationLag;

	UPROPERTY(EditAnywhere, Category="Rotation")
	FVector HorizontalRotationLag;
	UPROPERTY(EditAnywhere, Category="Rotation")
	FVector VerticalRotationLag;

	UPROPERTY(EditAnywhere, Category="Interpolation Settings")
	float Stiffness;
	UPROPERTY(EditAnywhere, Category="Interpolation Settings")
	float Damping;
	UPROPERTY(EditAnywhere, Category="Interpolation Settings")
	float Mass;
};

USTRUCT(BlueprintType)
struct FWeaponLagSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FWeaponLagSettingSection DefaultWeaponLag;

	UPROPERTY(EditAnywhere)
	FWeaponLagSettingSection ADSWeaponLag;
};

/**
 * The base class for all weapons in the game.
 */
UCLASS()
class FPEACE_API AFPCWeapon : public AFPCActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName WeaponAnimLayerClassName;

	FWeaponAnimSettings GetAnimSettings() const { return AnimSettings; }
	FWeaponLagSettings GetLagSettings() const { return LagSettings; }

	/*
	 * Setting up the initial values for the weapon instance based on selected target camera mode.
	 * The weapon will always be for this camera mode in its lifetime and the settings will be applied accordingly at this stage.
	 */
	void SetupWeapon(ECameraMode TargetCameraMode, USceneComponent* AttachCharacterMesh);

	/*
	 * Whether the weapon instance is visible to the owner of the character.
	 * The visibility to the world is not set here as it's already setup initially and should not change for the lifetime of this weapon instance.
	 */
	void ToggleVisibilityToOwner(bool bIsVisible);

	// --------------------------------- GETTERS ---------------------------------
	FTransform GetAimSocketTransform() const { return AimSocketActorSpaceTransform; }
	FTransform GetEmitterSocketTransform() const { return EmitterSocketActorSpaceTransform; }

protected:
	AFPCWeapon();

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ECameraMode UsedInCameraMode;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FWeaponAnimSettings AnimSettings;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FWeaponLagSettings LagSettings;

	UPROPERTY(BlueprintReadOnly)
	FTransform AimSocketActorSpaceTransform;

	UPROPERTY(BlueprintReadOnly)
	FTransform EmitterSocketActorSpaceTransform;

	/*
	 * Collection of mesh components that make up the weapon.
	 * Could be used to set up rendering, collision, etc.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UMeshComponent>> WeaponMeshComps;

	// --------------------- OVERRIDDEN FUNCTIONS ---------------------

	virtual void OnConstruction(const FTransform& Transform) override;

	// --------------------- OTHER FUNCTIONS ---------------------
	virtual TArray<TObjectPtr<UMeshComponent>> GatherWeaponMeshComps();
};
