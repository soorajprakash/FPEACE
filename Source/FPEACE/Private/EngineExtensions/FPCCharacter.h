// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "FPCCharacter.generated.h"

class UEnhancedInputComponent;
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLocomotionStateChanged, ELocomotionState, State);

UCLASS()
class FPEACE_API AFPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	--------------------- DELEGATE DEFINITION ---------------------
	FLocomotionStateChanged OnLocomotionStateChanged;

	//	--------------------- PUBLIC FUNCTIONS ---------------------

	/*
	 * Sets the Character to either use FPS camera mode or TPS camera mode
	 */
	UFUNCTION()
	void SetCameraMode(ECameraMode NewCameraMode);

	/*
	 * Changes the settings for the camera and the spring arm for the given mode using the character data
	 */
	void SetCameraSettings(ECameraMode CurrentCameraMode);

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
	 * Reference to the anim instance running on the FPS skeletal mesh component
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCAnimInstance> FPSMeshAnimInstance;

	/*
	 * Public getter for current movement direction
	 */
	ELocomotionDirection GetCurrentLocomotionDirection() const { return CurrentLocomotionDirection; }

	/*
	 * Get reference to the character data asset
	 */
	UFPCCharacterData* GetCharacterData();

protected:
	//	--------------------- ANIMATION FAST-PATH VARIABLES ---------------------

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterMoving;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterAccelerating;

	UPROPERTY(BlueprintReadOnly)
	float CurrentLocomotionStateFloat;

	UPROPERTY(BlueprintReadOnly)
	bool HasCharacterReachedCurrentMaxSpeed;

	//	--------------------- MOVEMENT DATA ---------------------

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterAcceleration2D;

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterVelocity2D;

	UPROPERTY(BlueprintReadOnly)
	float CharacterAbsoluteSpeed;

	UPROPERTY(BlueprintReadOnly)
	float CharacterAbsoluteSpeed2D;

	UPROPERTY(BlueprintReadOnly)
	float InputDirectionAngle;

	UPROPERTY(BlueprintReadOnly)
	float CurrentMaxLocomotionSpeed;

	UPROPERTY(BlueprintReadOnly)
	float CurrentDeltaDistance;

	/*
	 * This value will be used in the animation blueprint to modify the spine bone(s) to allow the character to look up or down
	 */
	UPROPERTY(BlueprintReadOnly)
	FRotator CharacterLookSpineVertical;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState currentLocomotionState;

	ELocomotionState TargetLocomotionState;

	UPROPERTY(EditDefaultsOnly)
	bool bIsCharacterArmed;

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

	//	--------------------- PROTECTED FUNCTIONS ---------------------

	void SetCurrentLocomotionState(ELocomotionState newLocomotionState);
	void SetTargetLocomotionState(ELocomotionState newLocomotionState);
	void SetCurrentLocomotionStateWithSettings(ELocomotionState newLocomotionState);
	void LinkCombatAnimClassToCharacter(FName AnimClassNameToLink);

	/*
	 * Calculate the direction enum from the given angle using direction limits in Character Data
	 */
	ELocomotionDirection CalculateLocomotionDirection(const float LocomotionDirectionAngle) const;

	void SetLocomotionDirection(ELocomotionDirection newLocomotionDirection);

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

	virtual void AddControllerPitchInput(float Val) override;

	virtual void AddControllerYawInput(float Val) override;

private:
	// Character direction limit values referenced from the Owning character data asset for ease of use
	FVector2D ForwardLimits;
	FVector2D BackwardLimits;
	float DeadZone;

	FVector LastWorldLocation;

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
	void ToggleRunSprint();

	/*
	 * Run/Sprint toggle input action binding
	 */
	void ToggleWalking();

	/*
	 * Set the movement component settings for a given locomotion state
	 */
	void SetLocomotionStateSettings(ELocomotionState newLocomotionState);

	/*
	 * Used to update derived variables for use in animation transition rules
	 * This allows the animation blueprint to be on FastPath
	 */
	void UpdateAnimationTransitionRuleValues();

	/*
	 * Tries to dynamically change states based on character speed and match it to the target state
	 */
	void DynamicLocomotionStateUpdate();
};
