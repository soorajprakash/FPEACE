// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCWeapon.h"
#include "FPCGun.generated.h"

class UFPCStaticMeshComponent;
class UFPCSkeletalMeshComponent;


/**
 * The base class for all guns in the game.
 */
UCLASS()
class AFPCGun : public AFPCWeapon
{
	GENERATED_BODY()

public:

	AFPCGun();
	
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

	// --------------------- OVERRIDDEN FUNCTIONS ---------------------

	virtual TArray<TObjectPtr<UMeshComponent>> GatherWeaponMeshComps() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
