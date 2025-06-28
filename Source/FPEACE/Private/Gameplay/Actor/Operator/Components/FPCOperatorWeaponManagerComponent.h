// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCOperatorComponentBase.h"
#include "AnimNotifies/FPCCharacterAnimationStateChangedNotify.h"
#include "Gameplay/Weapon/FPCWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "FPCOperatorWeaponManagerComponent.generated.h"

class FCTweenInstanceFloat;
class AFPCGun;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponEquipEvent, AFPCWeapon*, SpawnedFPSWeaponRef, AFPCWeapon*, SpawnedTPSWeaponRef);

USTRUCT(Blueprintable)
struct FWeaponSatchelItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<AFPCWeapon> FPSWeaponIns;

	UPROPERTY()
	TObjectPtr<AFPCWeapon> TPSWeaponIns;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPEACE_API UFPCOperatorWeaponManagerComponent : public UFPCOperatorComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPCOperatorWeaponManagerComponent();

	FWeaponEquipEvent OnNewWeaponEquipped;

	void UpdateWeaponVisibility(const bool IsInTPSCameraMode) const;

	void SwitchADSState(bool UseADS);

	void ToggleADSBlendFactor(const int targetBlendFactor);

	void ToggleWeaponUse(const bool UseWeapon);

	void TryCurrentGunReload() const;

	/*
	 * Move to the next/previous weapon in the satchel
	 * TRUE goes to the next weapon and FALSE goes to the previous
	 */
	void CycleWeaponInSatchel(bool bGoNext = true);

	UFUNCTION()
	void OnADSAnimStateChanged(ENotifyAnimationType AnimType, ENotifyAnimationEventType AnimEventType);

	// ----------------------------- GETTERS -----------------------------
	TObjectPtr<AFPCWeapon> GetCurrentFPSWeaponRef() const { return CurrentFPSWeaponRef; }
	TObjectPtr<AFPCGun> GetCurrentFPSGunRef() const { return CurrentFPSGunRef; }

	TObjectPtr<AFPCWeapon> GetCurrentTPSWeaponRef() const { return CurrentTPSWeaponRef; }
	TObjectPtr<AFPCGun> GetCurrentTPSGunRef() const { return CurrentTPSGunRef; }

	FWeaponAnimSettings GetCurrentWeaponAnimSettings() const { return CurrentWeaponAnimSettings; }

	bool GetIsCharacterArmed() const { return bIsCharacterArmed; }

	bool GetWantsToAds() const { return bWantsToADS; }

	bool GetIsCharacterInADSState() const { return bIsCharacterInADSState; }

	bool GetWantsToUseWeapon() const { return bWantsToUseWeapon; }

	bool GetWasWeaponUsedRecently() const { return bWasWeaponUsedRecently; }

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
	bool bWasWeaponUsedRecently = false;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCWeapon> CurrentFPSWeaponRef;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCGun> CurrentFPSGunRef;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCWeapon> CurrentTPSWeaponRef;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCGun> CurrentTPSGunRef;

	UPROPERTY(BlueprintReadOnly)
	TArray<FWeaponSatchelItem> WeaponSatchel;

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

	UPROPERTY(BlueprintReadOnly)
	bool RecentlyUsedWeaponStateChanged;

	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int currentWeaponSatchelIndex;


	FVectorSpringState LocationLagSpringState;
	FVectorSpringState RotationLagSpringState;

	FCTweenInstanceFloat* ADSBlendFactorTween;

	bool bLastFrameWantsADSState;
	bool bLastFrameWeaponRecentlyUsedState;

	FTimerHandle WeaponUseCoolDownTimer;

	UFUNCTION()
	void OnGunReloadStart(bool bEmptyReload, AFPCGun* ReloadingGun);

	UFUNCTION()
	void OnGunReloadComplete(AFPCGun* ReloadingGun);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(const FWeaponSatchelItem& SatchelItem);

	UFUNCTION()
	void GenerateSatchel();

	UFUNCTION()
	void AddNewWeaponToSatchel(const TSubclassOf<AFPCWeapon> WeaponBP);

	UFUNCTION()
	void CharacterCameraModeChanged(ECameraMode NewCameraMode);

	UFUNCTION()
	void CharacterCurrentLocomotionStateChanged(ELocomotionState NewLocomotionState);

	void SetCurrentWeaponHandIKOffset();

	void UpdateStateChanges();

	void CalculateCurrentWeaponLagValues();
};
