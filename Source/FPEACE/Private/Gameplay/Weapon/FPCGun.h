// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCWeapon.h"
#include "Gameplay/GAS/AttribueSets/FPCGunAttributeSet.h"
#include "FPCGun.generated.h"

class UFPCGunAbility_Fire;
class UFPCGunAttributeSet;
class URecoilHelper;
class UCurveVector;
class AFPCBullet;
class UFPCStaticMeshComponent;
class UFPCSkeletalMeshComponent;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMagWasEmptied, AFPCGun*, GunRef);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReloadStarted, bool, bEmptyReload, AFPCGun*, GunRef);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReloadFinished, AFPCGun*, GunRef);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemainingBulletsChanged, int32, RemainingBullets);

UENUM(BlueprintType)
enum EGunFireMode
{
	SingleShot,
	BurstFire,
	Automatic
};

USTRUCT(BlueprintType)
struct FGunSettings
{
	GENERATED_BODY()

	FGunSettings(): FireMode(SingleShot), BurstFireInterval(0.1f)
	{
	}

	/*
	 * The mode in which the gun will shoot
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EGunFireMode> FireMode;

	/*
	 * This is the cool-down time between each single bullet shot within a burst.
	 * This value is not used for single shot and automatic mode.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BurstFireInterval;
};

/**
 * The base class for all guns in the game.
 */
UCLASS()
class AFPCGun : public AFPCWeapon
{
	GENERATED_BODY()
	friend class UGunReloadedNotify;

public:
	AFPCGun();

	/*
	 * This is triggered the instant the last bullet in the magazine was fired.
	 */
	FOnMagWasEmptied OnMagWasEmptied;

	/*
	 * This is triggered the instant the last bullet in the magazine was fired or if the reload button was pressed.
	 */
	FOnReloadStarted OnReloadStarted;

	/*
	 * This is triggered when the reload animation finishes playing
	 */
	FOnReloadFinished OnReloadFinished;

	/*
	 * This is triggered whenever there's a change in the current number of bullets in the mag
	 */
	FOnRemainingBulletsChanged OnRemainingBulletsChanged;

	/*
	 * Spawn the Niagara system for the muzzle flash effect.
	 */
	void InitiateMuzzleFlash();

	/*
	 * Checks for conditions to fire the gun and fires the bullet.
	 */
	virtual void UseWeapon() override;

	/*
	 * Tries to sStart the reload animation and sets the gun to be in a reloading state.
	 * Won't work if the gun is already reloading or if the mag count is 0.
	 */
	void TryBeginReload();

	/*
	 * Called by the anim notify class
	 * Marks the reloading mag part of the animation to be finished
	 */
	void OnMagReloadFinishedPlaying();

	//	--------------------- GETTERS ---------------------

	TObjectPtr<UFPCSkeletalMeshComponent> GetReceiverMeshComp() const { return ReceiverMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetMagazineMeshComp() const { return MagazineMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetMuzzleMeshComp() const { return MuzzleMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetOpticMeshComp() const { return OpticMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetIronSightMeshComp() const { return IronSightMeshComp; }

	virtual UFPCSkeletalMeshComponent* GetBaseMeshComp() const override { return ReceiverMeshComp; }

	int32 GetCurrentRemainingBullets() const { return GunAttributeSet->GetRemainingBulletsInMag(); }

	int32 GetMagCapacity() const { return GunAttributeSet->GetMagCapacity(); }

	FGunSettings GetGunSettings() const { return GunSettings; }

	int32 GetTotalBulletsInUnusedMagazines() const { return GunAttributeSet->GetRemainingMags() * GunAttributeSet->GetMagCapacity(); }

	bool GetIsGunReloading() const { return bIsReloading; }

protected:
	//	--------------------- GUN ABILITIES ---------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UFPCGunAbility_Fire> FireAbility;

	//	--------------------- GUN ATTRIBUTES ---------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFPCGunAttributeSet> GunAttributeSet;

	//	--------------------- GUN COMPONENTS ---------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	TObjectPtr<UFPCSkeletalMeshComponent> ReceiverMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	TObjectPtr<UFPCStaticMeshComponent> MagazineMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	TObjectPtr<UFPCStaticMeshComponent> MuzzleMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	TObjectPtr<UFPCStaticMeshComponent> OpticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	TObjectPtr<UFPCStaticMeshComponent> IronSightMeshComp;


	//	--------------------- GUN SETTINGS ---------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun")
	FGunSettings GunSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun")
	TObjectPtr<UNiagaraSystem> MuzzleFlashEffect;

	/*
	 * Reference to the bullet class to be spawned when the gun is fired.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun")
	TSubclassOf<AFPCBullet> BulletClass;

	// --------------------- GUN FUNCTIONS ---------------------

	UFUNCTION(BlueprintCallable)
	void Fire();

	// --------------------- OVERRIDDEN FUNCTIONS ---------------------

	virtual TArray<TObjectPtr<UMeshComponent>> GatherWeaponMeshComps() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupWeapon(const ECameraMode TargetCameraMode, USceneComponent* AttachCharacterMesh) override;

private:
	bool bWasTriggerLiftedAfterLastFire = true;

	bool bIsReloading = false;

	//	--------------------- SETTERS ---------------------

	/*
	 * Used when the gun is in burst fire mode.
	 * Holds the number of shots remaining in the current single burst.
	 */
	int32 RemainingShotsInBurst;
	FTimerHandle GunContinuosFireHandle;
	void BurstModeFire();
	AFPCBullet* AcquireBullet();
};
