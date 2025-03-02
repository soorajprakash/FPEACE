// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "FPCCharacter.generated.h"

class UFPCCharacterAnimationManagerComponent;
class UFPCCharacterCameraManagerComponent;
class UInputAction;
class UFPCCharacterWeaponManagerComponent;
class AFPCPlayerController;
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

	//	--------------------- PUBLIC FUNCTIONS ---------------------

	/*
	 * Get the Movement component of this character
	 */
	TObjectPtr<UFPCCharacterMovementComponent> GetCharacterMovementComponent() const { return FPCMovementComp; }

	//	--------------------- GETTER FUNCTIONS ---------------------
	/*
	 * Get reference to the character data asset
	 */
	UFPCCharacterData* GetCharacterData();

	TObjectPtr<UFPCSkeletalMeshComponent> GetTPSBodyMeshComp() const
	{
		return TPSBodyMeshComp;
	}

	TObjectPtr<UFPCSkeletalMeshComponent> GetFPSBodyMeshComp() const
	{
		return FPSBodyMeshComp;
	}

	TObjectPtr<UFPCCapsuleComponent> GetFPCCapsuleComp() const
	{
		return FPCCapsuleComp;
	}

	TObjectPtr<UFPCCharacterMovementComponent> GetFPCMovementComp() const
	{
		return FPCMovementComp;
	}

	TObjectPtr<UFPCCameraComponent> GetFPCCameraComp() const
	{
		return FPCCameraComp;
	}

	TObjectPtr<UFPCSpringArmComponent> GetFPCSpringArmComp() const
	{
		return FPCSpringArmComp;
	}

	TObjectPtr<AFPCPlayerController> GetFPCPlayerController() const
	{
		return FPCPlayerControllerInstance;
	}

	TObjectPtr<UFPCCharacterWeaponManagerComponent> GetFPCCharacterWeaponManager() const
	{
		return FPCCharacterWeaponManagerComp;
	}

	TObjectPtr<UFPCCharacterCameraManagerComponent> GetFPCCharacterCameraManager() const
	{
		return FPCCameraManagerComp;
	}

	TObjectPtr<UFPCCharacterAnimationManagerComponent> GetFPCCharacterAnimationManager() const
	{
		return FPCCharacterAnimationManagerComp;
	}

protected:
	//	--------------------- COMPONENTS ---------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSkeletalMeshComponent> TPSBodyMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSkeletalMeshComponent> FPSBodyMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCCapsuleComponent> FPCCapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCCharacterMovementComponent> FPCMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCCameraComponent> FPCCameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSpringArmComponent> FPCSpringArmComp;

	/*
	 * Reference to the FPC Player Controller instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCPlayerController> FPCPlayerControllerInstance;

	/*
	 * Reference to the character data asset referenced in the game instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCCharacterData> FPCCharacterData;

	/*
	 * Reference to the Character's weapon manager component
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCCharacterWeaponManagerComponent> FPCCharacterWeaponManagerComp;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCCharacterCameraManagerComponent> FPCCameraManagerComp;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCCharacterAnimationManagerComponent> FPCCharacterAnimationManagerComp;

	//	--------------------- INPUT ---------------------

	/*
	 * Reference to the Look Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC/Inputs")
	TSoftObjectPtr<UInputAction> LookAction;

	/*
	 * Reference to the Move Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC/Inputs")
	TSoftObjectPtr<UInputAction> MoveAction;

	/*
	 * Reference to the Run Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC/Inputs")
	TSoftObjectPtr<UInputAction> RunAction;

	/*
	 * Reference to the Crouch Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC/Inputs")
	TSoftObjectPtr<UInputAction> CrouchAction;

	/*
	 * Reference to the Jump Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC/Inputs")
	TSoftObjectPtr<UInputAction> JumpAction;

	/*
	 * Reference to the ADS Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC/Inputs")
	TSoftObjectPtr<UInputAction> ADSAction;
	
	/*
	 * Reference to the Camera Switching Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC")
	TSoftObjectPtr<UInputAction> CameraSwitchAction;

	//	--------------------- OVERRIDES ---------------------

	virtual void PreInitializeComponents() override;

	/*
	 * When this character is possessed by the player controller
	 */
	virtual void PossessedBy(AController* NewController) override;

	/*
	 * Setup Player Input
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float Val) override;

	virtual void AddControllerYawInput(float Val) override;

private:
	
	/**
	 *Reference to Enhanced Input Component
	 */
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComp;

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

	void JumpStarted(const FInputActionValue& InputActionValue);

	void JumpEnded(const FInputActionValue& InputActionValue);
};
