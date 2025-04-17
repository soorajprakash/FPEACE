// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPCCharacterMovementComponent.generated.h"

class AFPCPlayerController;
class UFPCCapsuleComponent;
class UFPCCharacterWeaponManagerComponent;
class AFPCCharacter;
class UFPCCharacterData;
enum class ELocomotionDirection : uint8;
enum class ELocomotionState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLocomotionStateChangeDelegate, ELocomotionState, NewLocomotionState);

/**
 * An extension of the character movement component class of the character
 */
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class FPEACE_API UFPCCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	/*
	 * Event called when the current locomotion state has been changed
	 */
	FLocomotionStateChangeDelegate OnCurrentLocomotionStateChanged;

	/*
	 * Event called when the target locomotion state has been changed
	 */
	FLocomotionStateChangeDelegate OnTargetLocomotionStateChanged;

	void ToggleRunSprint();

	void ToggleCrouch();

	float GetCharacterAbsoluteSpeed2D() const { return CharacterAbsoluteSpeed2D; }
	
	float GetCharacterYawDelta() const { return CharacterYawDelta; }

	ELocomotionState GetCurrentLocomotionState() const { return currentLocomotionState; }

	void AddControllerPitchAndYawInput(float Pitch,float Yaw);

protected:
	UFPCCharacterMovementComponent();

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterAcceleration2D = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterVelocity2D = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	float CharacterVelocityZ = 0;

	UPROPERTY(BlueprintReadOnly)
	float LeanAngle = 0;

	UPROPERTY(BlueprintReadOnly)
	float YawAngularVelocity = 0;

	UPROPERTY(BlueprintReadOnly)
	float CharacterYawDelta = 0;

	UPROPERTY(BlueprintReadOnly)
	float CharacterAbsoluteSpeed = 0;

	UPROPERTY(BlueprintReadOnly)
	float CharacterAbsoluteSpeed2D = 0;

	UPROPERTY(BlueprintReadOnly)
	float CharacterNormalizedSpeed = 0;

	UPROPERTY(BlueprintReadOnly)
	float AccelerationDirectionAngle = 0;

	UPROPERTY(BlueprintReadOnly)
	float VelocityDirectionAngle = 0;

	UPROPERTY(BlueprintReadOnly)
	float CurrentDeltaDistance = 0;

	UPROPERTY(BlueprintReadOnly)
	float CurrentDeltaDistanceZ = 0;

	UPROPERTY(BlueprintReadOnly)
	float CurrentVelocityAccelerationDot = 0;

	/*
	 * Current distance from the character's lowest point to the ground straight below
	 * This value is only updated when the character is in the air
	 */
	UPROPERTY(BlueprintReadOnly)
	float DistanceToGroundWhileInAir = 0;

	/*
	 * The current locomotion direction of the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection CurrentVelocityDirection;

	/*
	 * The current direction of the owning character's acceleration
	 */
	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection CurrentAccelerationDirection;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState currentLocomotionState;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionStance currentLocomotionStance;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState TargetLocomotionState = ELocomotionState::Running;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState PrevLocomotionState;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionStance PrevLocomotionStance;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState PrevTargetLocomotionState;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection PrevVelocityDirection;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection PrevAccelerationDirection;

	UPROPERTY(BlueprintReadOnly)
	bool VelocityDirectionChanged = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterSlowingDown = false;

	UPROPERTY(BlueprintReadOnly)
	bool MovementStateChanged = false;

	UPROPERTY(BlueprintReadOnly)
	bool AccelerationStateChanged = false;

	UPROPERTY(BlueprintReadOnly)
	bool AccelerationDirectionChanged = false;

	UPROPERTY(BlueprintReadOnly)
	bool CurrentLocomotionStateChanged = false;

	UPROPERTY(BlueprintReadOnly)
	bool CurrentLocomotionStanceChanged = false;

	UPROPERTY(BlueprintReadOnly)
	bool TargetLocomotionStateChanged = false;

	UPROPERTY(BlueprintReadOnly)
	bool AnyMovementStateChanged = true;

	UPROPERTY(BlueprintReadOnly)
	bool PrevMovementState = false;

	UPROPERTY(BlueprintReadOnly)
	bool PrevAccelerationState = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterMoving = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterAccelerating = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterTurningInPlace = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterPivoting = false;

	UPROPERTY(BlueprintReadOnly)
	float TurnInPlaceStartingYaw = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsCharacterInProneState = false;

	UPROPERTY()
	TObjectPtr<AFPCCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UFPCCapsuleComponent> OwningCharacterCapsule;

	UPROPERTY()
	TObjectPtr<UFPCCharacterData> FPCCharacterData;

	UPROPERTY()
	TObjectPtr<AFPCPlayerController> FPCPlayerController;

	UPROPERTY()
	TObjectPtr<UFPCCharacterWeaponManagerComponent> FPCCharacterWeaponManager;

private:
	// Character direction limit values referenced from the Owning character data asset for ease of use
	FVector2D ForwardLimits = FVector2D::ZeroVector;
	FVector2D BackwardLimits = FVector2D::ZeroVector;
	float DeadZone = 0;

	FRotator PrevRotation;

	FVector LastWorldLocation = FVector::ZeroVector;

	float LastFrameMaxSpeed;
	ELocomotionDirection LastFrameAccelerationDirection;
	ELocomotionDirection LastFrameVelocityDirection;
	ELocomotionStance LastFrameLocomotionStance;
	ELocomotionState LastFrameLocomotionState;
	ELocomotionState LastFrameTargetLocomotionState;
	bool WasMovingLastFrame;
	bool WasAcceleratingLastFrame;

	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleLocomotionStateChange();
	void SetCurrentLocomotionState(ELocomotionState newLocomotionState);
	void SetTargetLocomotionState(ELocomotionState newLocomotionState);
	void SetCurrentLocomotionStateWithSettings(ELocomotionState newLocomotionState);
	void SetCurrentVelocityDirection(ELocomotionDirection newVelocityDirection);
	void SetCurrentAccelerationDirection(ELocomotionDirection newAccelerationDirection);

	/*
	 * Set the movement component settings for a given locomotion state
	 */
	void SetLocomotionStateSettings(ELocomotionState newLocomotionState);

	/*
	 * Sets the max walk speed using designated values in Character data from the current direction
	 */
	void UpdateDirectionalMaxWalkSpeed(ELocomotionDirection newVelocityDirection);

	/*
	 * Ticks each frame and checks if any movement states have changed
	 */
	void UpdateStateChanges();

	/*
	 * Calculate the direction enum from the given angle using direction limits in Character Data
	 */
	ELocomotionDirection CalculateLocomotionDirection(const float LocomotionDirectionAngle, const ELocomotionDirection CurrentDirection) const;

	/*
	 * Calculate the lean angle of the character.
	 * Mostly used to perform additive lean animations on necessary layers
	 */
	void CalculateLeanAngle();
};
