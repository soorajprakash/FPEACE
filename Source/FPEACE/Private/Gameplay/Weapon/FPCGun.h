// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCWeapon.h"
#include "FPCGun.generated.h"

class UFPCStaticMeshComponent;
class UFPCSkeletalMeshComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EGunFireMode> FireMode;

	/*
	 * This value is number of bullets per second for automatic fire mode.
	 * For burst fire, this value is number of bullets per burst.
	 * For single shot, this value is not used.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FireRate;
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

	//	--------------------- GETTERS ---------------------

	TObjectPtr<UFPCSkeletalMeshComponent> GetReceiverMeshComp() const { return ReceiverMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetMagazineMeshComp() const { return MagazineMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetMuzzleMeshComp() const { return MuzzleMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetOpticMeshComp() const { return OpticMeshComp; }

	TObjectPtr<UFPCStaticMeshComponent> GetIronSightMeshComp() const { return IronSightMeshComp; }

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

	// --------------------- OVERRIDDEN FUNCTIONS ---------------------

	virtual TArray<TObjectPtr<UMeshComponent>> GatherWeaponMeshComps() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
