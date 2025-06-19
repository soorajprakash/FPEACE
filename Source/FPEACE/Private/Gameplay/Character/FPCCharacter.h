// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "FPCCharacter.generated.h"

class AFPCSampleGameplayLevelScriptActor;
class UObjectPool;
class UFPCCharacterAnimationManagerComponent;
class UFPCCharacterCameraManagerComponent;
class UInputAction;
class UFPCCharacterWeaponManagerComponent;
class AFPCGameplayPlayerController;
class UFPCSpringArmComponent;
class UFPCCameraComponent;
class UFPCCapsuleComponent;
class UFPCSkeletalMeshComponent;
class UFPCCharacterData;
class UFPCCharacterMovementComponent;

UCLASS()
class FPEACE_API AFPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	--------------------- PUBLIC VARIABLES ---------------------
	UPROPERTY()
	TObjectPtr<UObjectPool> WorldObjectPool;


	//	--------------------- GETTER FUNCTIONS ---------------------
	/*
	 * Get reference to the character data asset
	 */
	TWeakObjectPtr<UFPCCharacterData> GetCharacterData();

	/*
	 * Get the Movement component of this character
	 */
	TWeakObjectPtr<UFPCCharacterMovementComponent> GetCharacterMovementComponent() const;

	TWeakObjectPtr<UFPCSkeletalMeshComponent> GetTPSBodyMeshComp() const;

	TWeakObjectPtr<UFPCSkeletalMeshComponent> GetFPSArmsMeshComp() const;

	TWeakObjectPtr<UFPCSkeletalMeshComponent> GetFPSLowerBodyMeshComp() const ;

	TWeakObjectPtr<UFPCCapsuleComponent> GetFPCCapsuleComp() const ;

	TWeakObjectPtr<UFPCCharacterMovementComponent> GetFPCMovementComp() const ;

	TWeakObjectPtr<UFPCCameraComponent> GetCharacterCameraComp() const ;

	TWeakObjectPtr<UFPCSpringArmComponent> GetFPCSpringArmComp() const ;

	TWeakObjectPtr<AFPCGameplayPlayerController> GetFPCPlayerController() const ;

	TWeakObjectPtr<UFPCCharacterWeaponManagerComponent> GetFPCCharacterWeaponManager() const ;

	TWeakObjectPtr<UFPCCharacterCameraManagerComponent> GetFPCCharacterCameraManager() const ;

	TWeakObjectPtr<UFPCCharacterAnimationManagerComponent> GetFPCCharacterAnimationManager() const ;

protected:
	//	--------------------- COMPONENTS ---------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSkeletalMeshComponent> TPSBodyMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSkeletalMeshComponent> FPSArmsMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSkeletalMeshComponent> FPSLowerBodyMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCCapsuleComponent> FPCCapsuleComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCCharacterMovementComponent> FPCMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCCameraComponent> CharacterCameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSpringArmComponent> FPCSpringArmComp;

	/*
	 * Reference to the Character's weapon manager component
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCCharacterWeaponManagerComponent> FPCCharacterWeaponManagerComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCCharacterCameraManagerComponent> FPCCameraManagerComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCCharacterAnimationManagerComponent> FPCCharacterAnimationManagerComp;

	/*
	 * Reference to the FPC Player Controller instance
	 */
	UPROPERTY(BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<AFPCGameplayPlayerController> FPCPlayerControllerInstance;

	/*
	 * Reference to the character data asset referenced in the game instance
	 */
	UPROPERTY(BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<UFPCCharacterData> FPCCharacterData;

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

	/*
	 * When this character is possessed by the player controller
	 */
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	/*
	 * Setup Player Input
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float Val) override;

	virtual void AddControllerYawInput(float Val) override;

private:
	//	--------------------- INPUT BINDING FUNCTIONS ---------------------

	/*
	 * Look input binding function
	 */
	void LookAround(const FInputActionValue& InputActionValue);

	/*
	 * Move input binding function
	 */
	void MoveAround(const FInputActionValue& InputActionValue);


	/*
	 * Walk/Run toggle input action binding
	 */
	void ToggleRunSprint();

	/*
	 * Input binding for toggling between crouching and standing
	 */
	void ToggleCrouch();

	void ToggleCameraMode();

	void ActivateADS();

	void DeactivateADS();

	void TriggerWeaponReload(const FInputActionValue& InputActionValue);

	void CycleWeaponUp();

	void CycleWeaponDown();

	void StartUsingWeapon();

	void StopUsingWeapon();

	void JumpStarted(const FInputActionValue& InputActionValue);

	void JumpEnded(const FInputActionValue& InputActionValue);
};
