// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "Gameplay/FPCActorComponent.h"
#include "Gameplay/Weapon/FPCWeapon.h"
#include "FPCCharacterWeaponManagerComponent.generated.h"

class UFPCCharacterMovementComponent;
class UFPCCharacterCameraManagerComponent;
class UFPCCharacterData;
class UFPCSkeletalMeshComponent;
class AFPCCharacter;
class AFPCWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponEquipEvent, AFPCWeapon*, SpawnedWeaponRef);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPEACE_API UFPCCharacterWeaponManagerComponent : public UFPCActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPCCharacterWeaponManagerComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsCharacterArmed = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsCharacterInADSState = false;

	FWeaponEquipEvent OnNewWeaponEquipped;

	void UpdateWeaponVisibility(const bool IsInTPSCameraMode) const;

	void SwitchADSState(bool UseADS);

protected:
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
	FVector CurrentWeaponHandIKLocationOffset;

	UPROPERTY(BlueprintReadOnly)
	FRotator CurrentWeaponHandIKRotationOffset;

	UPROPERTY(BlueprintReadOnly)
	bool AnyCombatStateChanged = true;

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
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

	FVector TargetWeaponHandIKLocationOffset;

	FRotator TargetWeaponHandIKRotationOffset;

	UFUNCTION(BlueprintCallable)
	void PickUpAndEquipWeapon(const TSoftClassPtr<AFPCWeapon>& WeaponBP);

	UFUNCTION()
	void CharacterCameraModeChanged(ECameraMode NewCameraMode);

	UFUNCTION()
	void CharacterCurrentLocomotionStateChanged(ELocomotionState NewLocomotionState);

	void SetTargetWeaponHandIKOffset();

	void UpdateCurrentWeaponHandIKOffset(const float DeltaTime);
};
