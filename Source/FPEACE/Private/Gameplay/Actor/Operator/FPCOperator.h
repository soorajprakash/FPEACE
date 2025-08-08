// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Actor/FPCCharacter.h"
#include "FPCOperator.generated.h"

struct FInputActionValue;
class UInputAction;
class UFPCOperatorAnimationManagerComponent;
class UFPCOperatorCameraManagerComponent;
class UFPCOperatorWeaponManagerComponent;
class UFPCSpringArmComponent;
class UFPCCameraComponent;
class UFPCCapsuleComponent;
class UFPCOperatorMovementComponent;
class UFPCOperatorData;

UCLASS()
class FPEACE_API AFPCOperator : public AFPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCOperator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnReceivedDamage(TWeakObjectPtr<AFPCCharacter> From, FName HitBone) override;

	//	--------------------- FUNCTIONS ---------------------
	void PlayEnemyHitRegisterSound(bool bIsKillShot = false) const;

	void PlayDamageGruntSound(bool bIsKillShot = false) const;

	//	--------------------- GETTER FUNCTIONS ---------------------

	/*
	 * Get reference to the character data asset
	 */
	TWeakObjectPtr<UFPCOperatorData> GetOperatorData();

	TWeakObjectPtr<AFPCGameplayPlayerController> GetFPCPlayerController() const;

	/*
	 * Get the Movement component of this character
	 */
	TWeakObjectPtr<UFPCOperatorMovementComponent> GetCharacterMovementComponent() const;

	TWeakObjectPtr<UFPCSkeletalMeshComponent> GetFPSArmsMeshComp() const;

	TWeakObjectPtr<UFPCSkeletalMeshComponent> GetFPSLowerBodyMeshComp() const;

	TWeakObjectPtr<UFPCCapsuleComponent> GetFPCCapsuleComp() const;

	TWeakObjectPtr<UFPCOperatorMovementComponent> GetFPCMovementComp() const;

	TWeakObjectPtr<UFPCCameraComponent> GetCharacterCameraComp() const;

	TWeakObjectPtr<UFPCSpringArmComponent> GetFPCSpringArmComp() const;

	TWeakObjectPtr<UFPCOperatorWeaponManagerComponent> GetFPCCharacterWeaponManager() const;

	TWeakObjectPtr<UFPCOperatorCameraManagerComponent> GetFPCCharacterCameraManager() const;

	TWeakObjectPtr<UFPCOperatorAnimationManagerComponent> GetFPCCharacterAnimationManager() const;

protected:
	/*
	 * Reference to the operator data asset referenced in the game instance
	 */
	UPROPERTY(BlueprintReadOnly, Category="FPEACE")
	TWeakObjectPtr<UFPCOperatorData> FPCOperatorData;

	/*
	 * Reference to the FPC Player Controller instance
	 */
	UPROPERTY(BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<AFPCGameplayPlayerController> FPCPlayerControllerInstance;

	/*
	 * The sound that is played when the enemy is hit by a bullet
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Audio")
	TObjectPtr<USoundBase> EnemyHitEffectSound;

	/*
	 * The sound that is played when the enemy is killed by a bullet
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Audio")
	TObjectPtr<USoundBase> EnemyKillEffectSound;

	/*
	 * The sound that is played when the player takes a hit
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Audio")
	TObjectPtr<USoundBase> DamageTakenGrunt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_Front;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_Back;

	//	--------------------- COMPONENTS ---------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSkeletalMeshComponent> FPSArmsMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSkeletalMeshComponent> FPSLowerBodyMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCCapsuleComponent> FPCCapsuleComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCOperatorMovementComponent> FPCMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCCameraComponent> CharacterCameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSpringArmComponent> FPCSpringArmComp;

	/*
	 * Reference to the Character's weapon manager component
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCOperatorWeaponManagerComponent> FPCCharacterWeaponManagerComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCOperatorCameraManagerComponent> FPCCameraManagerComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCOperatorAnimationManagerComponent> FPCCharacterAnimationManagerComp;

	//	--------------------- GAMEPLAY INPUT ---------------------

	/*
	 * Reference to the Look Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> LookAction;

	/*
	 * Reference to the Move Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> MoveAction;

	/*
	 * Reference to the Run Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> RunAction;

	/*
	 * Reference to the Crouch Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> CrouchAction;

	/*
	 * Reference to the Jump Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> JumpAction;

	/*
	 * Reference to the ADS Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> ADSAction;

	/*
	 * Reference to the Fire Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> FireAction;

	/*
	 * Reference to the Reload Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> ReloadAction;

	/*
	 * Reference to the Weapon Cycle Up Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> WeaponCycleUpAction;

	/*
	 * Reference to the Weapon Cycle Up Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> WeaponCycleDownAction;

	/*
	 * Reference to the Camera Switching Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TSoftObjectPtr<UInputAction> CameraSwitchAction;

	//	--------------------- OVERRIDES ---------------------

	virtual void PreInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void OnDeath_Implementation() override;
};
