// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ExtendedClasses/FPCActor.h"
#include "FPCWeapon.generated.h"

class AFPCOperator;
class AFPCGameplayPlayerController;
class UFPCOperatorAnimationManagerComponent;
class UFPCOperatorCameraManagerComponent;
class UFPCOperatorWeaponManagerComponent;
class UFPCOperatorMovementComponent;
class UFPCSkeletalMeshComponent;
enum class ELocomotionState : uint8;
class UFPCOperatorAnimInstance;
enum class ECameraMode : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSuccessfullyUsed, const AFPCWeapon*, WeaponRef);

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

	FWeaponLagSettingSection(): HorizontalLocationLag(), VerticalLocationLag(), HorizontalRotationLag(), VerticalRotationLag(), Stiffness(0), Damping(0), Mass(0)
	{
	}

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
	 * This is triggered when the weapon was successfully welded.
	 * When gun was fired, when mêlée weapon was swung, etc.
	 */
	FOnWeaponSuccessfullyUsed OnWeaponSuccessfullyUsed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<UTexture2D> WeaponHUDIcon;

	/*
	 * The camera mode for which this particular weapon instance is used for
	 */
	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ECameraMode UsedInCameraMode;

	/*
	 * Generic function to be called when the weapon is used.
	 * Guns can fire bullets, mêlée weapons can be swiped, fists can be used to punch, etc.
	 */
	virtual void UseWeapon();

	/*
	 * Setting up the initial values for the weapon instance based on selected target camera mode.
	 * The weapon will always be for this camera mode in its lifetime and the settings will be applied accordingly at this stage.
	 */
	virtual void SetupWeapon(const ECameraMode TargetCameraMode, USceneComponent* AttachCharacterMesh);

	UFUNCTION(BlueprintCallable)
	virtual UFPCSkeletalMeshComponent* GetBaseMeshComp() const { return nullptr; }

	/*
	 * Whether the weapon instance is visible to the owner of the character.
	 * The visibility to the world is not set here as it's already setup initially and should not change for the lifetime of this weapon instance.
	 */
	void ToggleVisibilityToOwner(bool bIsVisible);

	// --------------------------------- GETTERS ---------------------------------
	FTransform GetAimSocketTransform() const { return AimSocketActorSpaceTransform; }
	FTransform GetEmitterSocketTransform() const { return EmitterSocketActorSpaceTransform; }
	bool GetCameraModeMatchesWeapon() const { return bCameraModeMatchesWeapon; }

	// --------------------------------- SETTERS ---------------------------------

	void SetIsWeaponInUse(bool bIsInUse) { bIsWeaponInUse = bIsInUse; }

protected:
	AFPCWeapon();

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FWeaponAnimSettings AnimSettings;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FWeaponLagSettings LagSettings;

	/*
	 * Holds the state of whether or not the current camera mode matches this weapon's UsedInCameraMode value
	 * Used to know if this weapon instance is thought of by the user as their current weapon
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bCameraModeMatchesWeapon;

protected:
	UPROPERTY(BlueprintReadOnly)
	FTransform AimSocketActorSpaceTransform;

	UPROPERTY(BlueprintReadOnly)
	FTransform EmitterSocketActorSpaceTransform;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWeaponReadyToBeUsed = true;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWeaponInCoolDown = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWeaponInUse;

	/*
	 * Reference to the character that is actively using this weapon
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AFPCOperator> OwningOperator;

	/*
	 * Reference to the skeletal mesh that is actively using this weapon
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCSkeletalMeshComponent> OwningMesh;

	/*
	 * Reference to the movement component in the character that is actively using this weapon
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCOperatorMovementComponent> OwningCharacterMovementComponent;

	/*
	 * Reference to the weapon manager component in the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCOperatorWeaponManagerComponent> OwningCharacterWeaponManager;

	/*
	 * Reference to the camera manager component in the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCOperatorCameraManagerComponent> OwningCharacterCameraManager;

	/*
	 * Reference to the animation manager component in the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCOperatorAnimationManagerComponent> OwningCharacterAnimationManager;


	/*
	 * Collection of mesh components that make up the weapon.
	 * Could be used to set up rendering, collision, etc.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UMeshComponent>> WeaponMeshComps;

	// --------------------- OVERRIDDEN FUNCTIONS ---------------------

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;

	// --------------------- OTHER FUNCTIONS ---------------------
	virtual TArray<TObjectPtr<UMeshComponent>> GatherWeaponMeshComps();

	UFUNCTION()
	virtual void OnCameraModeChanged(ECameraMode NewCameraMode);
};
