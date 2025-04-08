// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCWeapon.h"
#include "FPCGun.generated.h"

class AFPCBullet;
class UFPCStaticMeshComponent;
class UFPCSkeletalMeshComponent;
class UNiagaraSystem;

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

	FGunSettings(): FireMode(SingleShot), FireRate(10), FireCoolDownInterval(0.3f), BurstFireInterval(0.1f), MagCapacity(10)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EGunFireMode> FireMode;

	/*
	 * This value is the number of bullets per second for automatic fire mode.
	 * For burst fire, this value is the number of bullets per burst.
	 * For a single shot, the inverse of this value is used as the cool-down time
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FireRate;

	/*
	 * This is the cool-down time between each single bullet shot.
	 * This value is not used for single shot mode.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FireCoolDownInterval;

	/*
	 * This is the cool-down time between each single bullet shot within a burst.
	 * This value is not used for single shot mode.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BurstFireInterval;

	/*
	 * Number of bullets in the magazine.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MagCapacity;

	/*
	 * The velocity of the fired bullet in centimeters per second.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BulletVelocity = 35000;
};

/**
 * The base class for all guns in the game.
 */
UCLASS()
class AFPCGun : public AFPCWeapon
{
	GENERATED_BODY()

public:
	AFPCGun();

	/*
	 * Spawn the Niagara system for the muzzle flash effect.
	 */
	void InitiateMuzzleFlash();

	/*
	 * Checks for conditions to fire the gun and fires the bullet.
	 */
	virtual void UseWeapon() override;

	//	--------------------- GETTERS ---------------------

	TObjectPtr<UFPCSkeletalMeshComponent> GetReceiverMeshComp() const { return ReceiverMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetMagazineMeshComp() const { return MagazineMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetMuzzleMeshComp() const { return MuzzleMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetOpticMeshComp() const { return OpticMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetIronSightMeshComp() const { return IronSightMeshComp; }

	virtual UFPCSkeletalMeshComponent* GetBaseMeshComp() const override { return ReceiverMeshComp; }

protected:
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

	// --------------------- OVERRIDDEN FUNCTIONS ---------------------

	virtual TArray<TObjectPtr<UMeshComponent>> GatherWeaponMeshComps() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupWeapon(const ECameraMode TargetCameraMode, USceneComponent* AttachCharacterMesh) override;

private:
	bool bWasTriggerLiftedAfterLastFire = true;

	/*
	 * Used when the gun is in burst fire mode.
	 * Holds the number of shots remaining in the current single burst.
	 */
	int RemainingShotsInBurst;
	FTimerHandle GunContinuosFireHandle;
	FTimerHandle GunCoolDownHandle;
	void Fire();
	void BurstModeFire();
	AFPCBullet* AcquireBullet() const;
};
