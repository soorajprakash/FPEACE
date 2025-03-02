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


USTRUCT(Blueprintable)
struct FWeaponAnimSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<ELocomotionState, FTransform> DefaultLocomotionStateOffsets;

	UPROPERTY(EditDefaultsOnly)
	TMap<ELocomotionState, FTransform> ADSLocomotionStateOffsets;
};


/**
 * The base class for all weapons in the game.
 */
UCLASS()
class FPEACE_API AFPCWeapon : public AFPCActor
{
	GENERATED_BODY()

public:
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName WeaponAnimLayerClassName;

	FWeaponAnimSettings GetAnimSettings() const { return AnimSettings; }

protected:
	AFPCWeapon();

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ECameraMode UsedInCameraMode;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FWeaponAnimSettings AnimSettings;

protected:
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
