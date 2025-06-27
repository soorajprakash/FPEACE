// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/FPCActorComponent.h"
#include "FPCCharacterAnimationManagerComponent.generated.h"

enum class ECameraMode : uint8;
class AFPCWeapon;
class UFPCCharacterWeaponManagerComponent;
class UFPCCharacterMovementComponent;
class UFPCCharacterData;
class AFPCOperator;
class AFPCGameplayPlayerController;
class UFPCLayerAnimInstance;
class UFPCSkeletalMeshComponent;
enum class ELocomotionState : uint8;
enum class ELocomotionDirection : uint8;
class UFPCSkeletalAnimInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPEACE_API UFPCCharacterAnimationManagerComponent : public UFPCActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPCCharacterAnimationManagerComponent();

	UFPCSkeletalAnimInstance* GetTPSMeshAnimInstance() const { return TPSMeshAnimInstance; }
	UFPCSkeletalAnimInstance* GetFPSMeshAnimInstance() const { return FPSMeshAnimInstance; }

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
	 * Reference to the anim instance running on the base skeletal mesh component
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCSkeletalAnimInstance> TPSMeshAnimInstance;

	/*
	 * Reference to the anim instance running on the FPS skeletal mesh component
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCSkeletalAnimInstance> FPSMeshAnimInstance;

	/*
	 * This value will be used in the animation blueprint to modify the spine bone(s) to allow the character to look up or down
	 */
	UPROPERTY(BlueprintReadOnly)
	FRotator CharacterLookSpineVertical = FRotator::ZeroRotator;

private:
	UPROPERTY()
	TWeakObjectPtr<AFPCOperator> OwningOperator;

	UPROPERTY()
	TWeakObjectPtr<AFPCGameplayPlayerController> PlayerControllerRef;

	UPROPERTY()
	TWeakObjectPtr<UFPCCharacterData> FPCCharacterData;

	UPROPERTY()
	TWeakObjectPtr<UFPCSkeletalMeshComponent> TPSBodyMeshComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCSkeletalMeshComponent> FPSBodyMeshComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCCharacterMovementComponent> FPCCharacterMovementComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCCharacterWeaponManagerComponent> FPCWeaponManagerComp;

	void LinkCombatAnimClassToCharacter(FName AnimClassNameToLink) const;

	TSoftClassPtr<UFPCLayerAnimInstance> GetAnimClassFor(ECameraMode TargetCameraMode, FName AnimStateName, const FString& ReasonForGettingThisAnim) const;

	UFUNCTION()
	void OnGunReloadStart(bool bEmptyReload, AFPCGun* ReloadingGun);

	UFUNCTION()
	void OnEquipNewWeapon(AFPCWeapon* SpawnedFPSWeapon, AFPCWeapon* SpawnedTPSWeapon);

	UFUNCTION()
	void OnCurrentFPSWeaponUsed();

	UFUNCTION()
	void OnCurrentTPSWeaponUsed();
};
