// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPCCharacterMovementComponent.generated.h"

class UFPCCharacterWeaponManagerComponent;
class AFPCCharacter;
class UFPCCharacterData;
enum class ELocomotionDirection : uint8;
enum class ELocomotionState : uint8;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class FPEACE_API UFPCCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UFPCCharacterMovementComponent();

protected:
	UPROPERTY()
	TObjectPtr<AFPCCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UFPCCharacterData> FPCCharacterData;

	UPROPERTY()
	TObjectPtr<UFPCCharacterWeaponManagerComponent> FPCCharacterWeaponManager;

public:
	UPROPERTY(BlueprintReadOnly)
	FVector CharacterAcceleration2D = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterVelocity2D = FVector::ZeroVector;

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
	float CurrentVelocityAccelerationDot = 0;

	/*
	 * The current locomotion direction of the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection CurrentVelocityDirection = ELocomotionDirection::Forward;

	/*
	 * The current direction of the owning character's acceleration
	 */
	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection CurrentAccelerationDirection = ELocomotionDirection::Forward;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState currentLocomotionState = ELocomotionState::Running;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState TargetLocomotionState = ELocomotionState::Running;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState PrevLocomotionState = ELocomotionState::Running;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState PrevTargetLocomotionState = ELocomotionState::Running;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection PrevVelocityDirection = ELocomotionDirection::Forward;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection PrevAccelerationDirection = ELocomotionDirection::Forward;

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

	void ToggleRunSprint();

	void ToggleCrouch();

private:
	// Character direction limit values referenced from the Owning character data asset for ease of use
	FVector2D ForwardLimits = FVector2D::ZeroVector;
	FVector2D BackwardLimits = FVector2D::ZeroVector;
	float DeadZone = 0;

	float PrevYaw = 0;

	FVector LastWorldLocation = FVector::ZeroVector;

	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleLocomotionStateChange();
	void SetCurrentLocomotionState(ELocomotionState newLocomotionState);
	void SetCurrentLocomotionStateWithSettings(ELocomotionState newLocomotionState);
	void SetCurrentVelocityDirection(ELocomotionDirection newVelocityDirection);
	void SetCurrentAccelerationDirection(ELocomotionDirection newAccelerationDirection);

	/*
	 * Set the movement component settings for a given locomotion state
	 */
	void SetLocomotionStateSettings(ELocomotionState newLocomotionState);

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
