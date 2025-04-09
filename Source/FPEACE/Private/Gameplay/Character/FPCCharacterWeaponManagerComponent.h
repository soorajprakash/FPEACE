// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "AnimNotifies/FPCCharacterAnimationStateChangedNotify.h"
#include "Gameplay/FPCActorComponent.h"
#include "Gameplay/Weapon/FPCWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "FPCCharacterWeaponManagerComponent.generated.h"

class UFPCCharacterAnimationManagerComponent;
class FCTweenInstanceFloat;
class UFPCCharacterMovementComponent;
class UFPCCharacterCameraManagerComponent;
class UFPCCharacterData;
class UFPCSkeletalMeshComponent;
class AFPCCharacter;
class AFPCWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponEquipEvent, AFPCWeapon*, SpawnedFPSWeaponRef, AFPCWeapon*, SpawnedTPSWeaponRef);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPEACE_API UFPCCharacterWeaponManagerComponent : public UFPCActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPCCharacterWeaponManagerComponent();

	FWeaponEquipEvent OnNewWeaponEquipped;

	void UpdateWeaponVisibility(const bool IsInTPSCameraMode) const;

	void SwitchADSState(bool UseADS);

	void ToggleWeaponUse(const bool UseWeapon);

	UFUNCTION()
	void OnADSAnimStateChanged(ENotifyAnimationType AnimType, ENotifyAnimationEventType AnimEventType);

	// ----------------------------- GETTERS -----------------------------
	TObjectPtr<AFPCWeapon> GetCurrentFPSWeaponRef() const { return CurrentFPSWeaponRef; }

	TObjectPtr<AFPCWeapon> GetCurrentTPSWeaponRef() const { return CurrentTPSWeaponRef; }

	FWeaponAnimSettings GetCurrentWeaponAnimSettings() const { return CurrentWeaponAnimSettings; }

	bool GetIsCharacterArmed() const { return bIsCharacterArmed; }

	bool GetWantsToAds() const { return bWantsToADS; }

	bool GetIsCharacterInADSState() const { return bIsCharacterInADSState; }

	bool GetWantsToUseWeapon() const { return bWantsToUseWeapon; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsCharacterArmed = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCharacterInADSState = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsADSInProgress = false;

	UPROPERTY(BlueprintReadOnly)
	bool bWantsToADS = false;

	UPROPERTY(BlueprintReadOnly)
	bool bWantsToUseWeapon = false;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCWeapon> CurrentFPSWeaponRef;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCWeapon> CurrentTPSWeaponRef;

	/*
	 * Reference to the weapon animation settings struct from the current weapon that is equipped
	 */
	UPROPERTY(BlueprintReadOnly)
	FWeaponAnimSettings CurrentWeaponAnimSettings;

	UPROPERTY(BlueprintReadOnly)
	FWeaponLagSettings CurrentWeaponLagSettings;

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentWeaponLocationLag;

	UPROPERTY(BlueprintReadOnly)
	FRotator CurrentWeaponRotationLag;

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentWeaponRotationLagVector;

	UPROPERTY(BlueprintReadOnly)
	FTransform CurrentWeaponEmitterSocketOffset;

	UPROPERTY(BlueprintReadOnly)
	FTransform CurrentWeaponAimSocketOffset;

	UPROPERTY(BlueprintReadOnly)
	float CurrentADSBlendFactor;

	UPROPERTY(BlueprintReadWrite)
	FVector CurrentWeaponHandIKLocationOffset;

	UPROPERTY(BlueprintReadWrite)
	FRotator CurrentWeaponHandIKRotationOffset;

	UPROPERTY(BlueprintReadOnly)
	bool ADSStateChanged;

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVectorSpringState LocationLagSpringState;
	FVectorSpringState RotationLagSpringState;

	FCTweenInstanceFloat* ADSBlendFactorTween;

	bool bLastFrameWantsADSState;

	UPROPERTY()
	TObjectPtr<AFPCCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UFPCCharacterMovementComponent> OwningCharacterMovementComp;

	UPROPERTY()
	TObjectPtr<UFPCCharacterData> FPCCharacterData;

	UPROPERTY()
	TObjectPtr<UFPCSkeletalMeshComponent> TPSBodyMeshComp;

	UPROPERTY()
	TObjectPtr<UFPCSkeletalMeshComponent> FPSBodyMeshComp;

	UPROPERTY()
	TObjectPtr<UFPCCharacterCameraManagerComponent> FPCCameraManagerComp;

	UPROPERTY()
	TObjectPtr<UFPCCharacterAnimationManagerComponent> FPCAnimationManagerComp;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(const TSoftClassPtr<AFPCWeapon>& WeaponBP);

	UFUNCTION()
	void CharacterCameraModeChanged(ECameraMode NewCameraMode);

	UFUNCTION()
	void CharacterCurrentLocomotionStateChanged(ELocomotionState NewLocomotionState);

	void SetCurrentWeaponHandIKOffset();

	void UpdateStateChanges();

	void CalculateCurrentWeaponLagValues();
};
