// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPCAnimInstance.generated.h"

class UFPCCharacterData;
enum class ELocomotionDirection : uint8;
enum class ELocomotionState : uint8;
class AFPCCharacter;
enum class ECameraMode : uint8;
class UFPCCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class FPEACE_API UFPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	/*
	 * This is used to only perform operations on the base anim instance for optimization
	 */
	bool isBaseAnimInstance;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCCharacter> OwningCharacter;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCCharacterMovementComponent> OwningCharacterMovementComponent;

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
	float LeanAngle;

	void SetCurrentLocomotionState(ELocomotionState newLocomotionState);

	ELocomotionState GetCurrentLocomotionState() const {return currentLocomotionState;}

	TSoftClassPtr<UFPCAnimInstance> GetAnimClassFor(ECameraMode TargetCameraMode, FName AnimStateName, const FString& ReasonForGettingThisAnim);

protected:
	/*
	 * The current locomotion direction of the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	ELocomotionDirection CurrentLocomotionDirection;

	UPROPERTY(BlueprintReadOnly)
	ELocomotionState currentLocomotionState;

	TObjectPtr<UFPCCharacterData> GetCharacterData();

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	/*
	 * Reference to the character data asset referenced in the game instance
	 */
	TObjectPtr<UFPCCharacterData> OwningCharacterData;

	/*
	 * Used to calculate angular velocity
	 */
	float previousYaw;

	// Character direction limit values referenced from the Owning character data asset for ease of use
	FVector2D ForwardLimits;
	FVector2D BackwardLimits;
	float DeadZone;

	/*
	 * Calculate the direction enum from the given angle using direction limits in Character Data
	 */
	ELocomotionDirection GetLocomotionDirection(float LocomotionDirectionAngle) const;

	void CalculateLeanAngle(float DeltaSeconds);
};
