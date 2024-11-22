// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "FPCCharacter.generated.h"

class UFPCCharacterData;
enum class ELocomotionState : uint8;
class UFPCAnimInstance;
enum class ECameraMode : uint8;
class AFPCPlayerController;
class AFPCGameMode;
class UInputAction;
class UFPCSpringArmComponent;
class UFPCCameraComponent;
class UInputMappingContext;
class UFPCCharacterMovementComponent;
class UFPCCapsuleComponent;
class UFPCSkeletalMeshComponent;

UCLASS()
class FPEACE_API AFPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 * Sets the Character to either use FPS camera mode or TPS camera mode
	 */
	UFUNCTION()
	void SetCameraMode(ECameraMode NewCameraMode);

	/*
	 * Get the Movement component of this character
	 */
	TObjectPtr<UFPCCharacterMovementComponent> GetCharacterMovementComponent() const { return FPCMovementComp; }

	/*
	 * Reference to the anim instance running on the base skeletal mesh component
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCAnimInstance> BaseMeshAnimInstance;

	/*
	 * Public getter for current movement direction
	 */
	ELocomotionDirection GetCurrentLocomotionDirection() const { return CurrentLocomotionDirection; }

protected:
	//	--------------------- DATA ---------------------

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterVelocity;

	UPROPERTY(BlueprintReadOnly)
	float CharacterAbsoluteSpeed;

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterVelocity2D;

	UPROPERTY(BlueprintReadOnly)
	float CharacterAbsoluteSpeed2D;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterMoving;

	UPROPERTY(BlueprintReadOnly)
	float currentLocomotionStateFloat;

	UPROPERTY(BlueprintReadOnly)
	float DirectionAngle;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState currentLocomotionState;

	/*
	 * The current locomotion direction of the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection CurrentLocomotionDirection;

	//	--------------------- COMPONENTS ---------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSkeletalMeshComponent> BaseMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSkeletalMeshComponent> TPSBodyMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSkeletalMeshComponent> FPSBodyMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSkeletalMeshComponent> FPSArmsMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCCapsuleComponent> FPCCapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCCharacterMovementComponent> FPCMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCCameraComponent> FPCCameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSpringArmComponent> FPCSpringArmComp;

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
	 * Reference to the Sprint Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC/Inputs")
	TSoftObjectPtr<UInputAction> SprintAction;

	//	--------------------- OTHER REFS ---------------------
	/*
	 * Reference to the FPC Player Controller instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCPlayerController> FPCPlayerControllerInstance;

	//	--------------------- FUNCTIONS ---------------------

	void SetCurrentLocomotionState(ELocomotionState newLocomotionState);

	/*
	 * Calculate the direction enum from the given angle using direction limits in Character Data
	 */
	ELocomotionDirection CalculateLocomotionDirection(const float LocomotionDirectionAngle) const;

	//	--------------------- OVERRIDES ---------------------

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/*
	 * When this character is possessed by the player controller
	 */
	virtual void PossessedBy(AController* NewController) override;

	/*
	 * Setup Player Input
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	// Character direction limit values referenced from the Owning character data asset for ease of use
	FVector2D ForwardLimits;
	FVector2D BackwardLimits;
	float DeadZone;

	/*
	 * Reference to the character data asset referenced in the game instance
	 */
	TObjectPtr<UFPCCharacterData> FPCCharacterData;

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
	void ToggleWalkRun();

	/*
	 * Run/Sprint toggle input action binding
	 */
	void ToggleSprint();

	/*
	 * Set the movement component settings for a given locomotion state
	 */
	void SetLocomotionStateSettings(ELocomotionState newLocomotionState) const;

	void CheckIfCharacterMoving();
};
