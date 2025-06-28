// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCOperatorComponentBase.h"
#include "FPCCharacterAnimationManagerComponent.generated.h"

class UFPCOperatorLayerAnimInstance;
class AFPCWeapon;
enum class ECameraMode : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPEACE_API UFPCCharacterAnimationManagerComponent : public UFPCOperatorComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPCCharacterAnimationManagerComponent();

	UFPCOperatorSkeletalAnimInstance* GetTPSMeshAnimInstance() const { return TPSMeshAnimInstance; }
	UFPCOperatorSkeletalAnimInstance* GetFPSMeshAnimInstance() const { return FPSMeshAnimInstance; }

protected:
	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	float CurrentLocomotionStateFloat = 0;

	UPROPERTY(BlueprintReadOnly)
	bool HasCharacterReachedCurrentMaxSpeed = false;

	UPROPERTY(BlueprintReadOnly)
	float CurrentAnimPlayRate = false;

	/*
	 * This value will be used in the animation blueprint to modify the spine bone(s) to allow the character to look up or down
	 */
	UPROPERTY(BlueprintReadOnly)
	FRotator CharacterLookSpineVertical = FRotator::ZeroRotator;

private:

	void LinkCombatAnimClassToCharacter(FName AnimClassNameToLink) const;

	TSoftClassPtr<UFPCOperatorLayerAnimInstance> GetAnimClassFor(ECameraMode TargetCameraMode, FName AnimStateName, const FString& ReasonForGettingThisAnim) const;

	UFUNCTION()
	void OnGunReloadStart(bool bEmptyReload, AFPCGun* ReloadingGun);

	UFUNCTION()
	void OnEquipNewWeapon(AFPCWeapon* SpawnedFPSWeapon, AFPCWeapon* SpawnedTPSWeapon);

	UFUNCTION()
	void OnCurrentFPSWeaponUsed();

	UFUNCTION()
	void OnCurrentTPSWeaponUsed();
};
