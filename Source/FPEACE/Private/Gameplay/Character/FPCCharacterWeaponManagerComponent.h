// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "Gameplay/FPCActorComponent.h"
#include "FPCCharacterWeaponManagerComponent.generated.h"

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

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AFPCWeapon> CurrentFPSWeaponRef;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AFPCWeapon> CurrentTPSWeaponRef;
	
	virtual void InitializeComponent() override;

private:
	UPROPERTY()
	TObjectPtr<AFPCCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UFPCCharacterData> FPCCharacterData;

	UPROPERTY()
	TObjectPtr<UFPCSkeletalMeshComponent> TPSBodyMeshComp;

	UPROPERTY()
	TObjectPtr<UFPCSkeletalMeshComponent> FPSBodyMeshComp;

	UPROPERTY()
	TObjectPtr<UFPCCharacterCameraManagerComponent> FPCCameraManagerComp;

	UFUNCTION(BlueprintCallable)
	void PickUpAndEquipWeapon(const TSubclassOf<AFPCWeapon>& WeaponBP);

	UFUNCTION()
	void CharacterCameraModeChanged(ECameraMode NewCameraMode);
};
