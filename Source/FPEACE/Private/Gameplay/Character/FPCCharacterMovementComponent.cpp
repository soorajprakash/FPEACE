// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCCharacterMovementComponent.h"
#include "FPCCharacter.h"
#include "FPCCharacterWeaponManagerComponent.h"
#include "KismetAnimationLibrary.h"
#include "DataStructures/FPCCharacterData.h"
#include "Kismet/KismetMathLibrary.h"

struct FLocomotionStateSetting;

UFPCCharacterMovementComponent::UFPCCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UFPCCharacterMovementComponent::ToggleRunSprint()
{
	TargetLocomotionState = TargetLocomotionState != ELocomotionState::Running ? ELocomotionState::Running : ELocomotionState::Sprinting;
}

void UFPCCharacterMovementComponent::ToggleCrouch()
{
	bWantsToCrouch = !bWantsToCrouch;
}

void UFPCCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (!OwningCharacter)
		OwningCharacter = Cast<AFPCCharacter>(GetOwner());

	if (OwningCharacter)
	{
		FPCCharacterData = OwningCharacter->GetCharacterData();
		FPCCharacterWeaponManager = OwningCharacter->GetFPCCharacterWeaponManager();
	}

	// Store required Character data
	if (FPCCharacterData)
	{
		ForwardLimits = FPCCharacterData->CharacterDirectionLimits.ForwardLimits;
		BackwardLimits = FPCCharacterData->CharacterDirectionLimits.BackwardLimits;
		DeadZone = FPCCharacterData->CharacterDirectionLimits.DirectionalDeadzone;
	}
}

void UFPCCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Fetch movement data
	const FVector& CharacterAcceleration = GetCurrentAcceleration();

	CharacterAcceleration2D = FVector(CharacterAcceleration.X, CharacterAcceleration.Y, 0);
	CharacterVelocity2D = FVector(Velocity.X, Velocity.Y, 0);
	CharacterAbsoluteSpeed = UKismetMathLibrary::VSizeXY(Velocity);
	CharacterAbsoluteSpeed2D = UKismetMathLibrary::VSizeXY(CharacterVelocity2D);
	CharacterNormalizedSpeed = CharacterAbsoluteSpeed / MaxWalkSpeed;

	//Calculate the direction angles
	if (!FMath::IsNearlyZero(CharacterAcceleration2D.Size()))
	{
		AccelerationDirectionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterAcceleration2D, OwningCharacter->GetActorRotation());
		SetCurrentAccelerationDirection(CalculateLocomotionDirection(AccelerationDirectionAngle, CurrentAccelerationDirection));
	}

	if (!FMath::IsNearlyZero(CharacterVelocity2D.Size()))
	{
		VelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterVelocity2D, OwningCharacter->GetActorRotation());
		SetCurrentVelocityDirection(CalculateLocomotionDirection(VelocityDirectionAngle, CurrentVelocityDirection));
	}
	
	// Update if the character is moving
	if (IsCharacterMoving && CharacterVelocity2D.Length() < 1)
		IsCharacterMoving = false;
	else if (!IsCharacterMoving && CharacterVelocity2D.Length() > 10)
		IsCharacterMoving = true;

	IsCharacterAccelerating = !CharacterAcceleration2D.IsNearlyZero();

	// Calculate the current delta distance covered by the character
	FVector CurrentWorldLocation = GetActorLocation();
	CurrentDeltaDistance = FVector::Distance(LastWorldLocation, CurrentWorldLocation);
	LastWorldLocation = CurrentWorldLocation;

	// Precompute normalized velocity and acceleration for clarity
	const FVector NormalizedVelocity = CharacterVelocity2D.GetSafeNormal();
	const FVector NormalizedAcceleration = CharacterAcceleration2D.GetSafeNormal();

	// Calculate the dot product once
	CurrentVelocityAccelerationDot = FVector::DotProduct(NormalizedVelocity, NormalizedAcceleration);

	// Calculate Yaw Angular Velocity
	float CurrentYaw = OwningCharacter->GetActorRotation().Yaw;
	CharacterYawDelta = CurrentYaw - PrevYaw;
	YawAngularVelocity = CharacterYawDelta / DeltaTime;
	PrevYaw = CurrentYaw;

	//	--------------------- CHECK FOR PIVOTING ---------------------

	if (IsCharacterPivoting)
	{
		// Check if the character has stopped pivoting or needs to trigger a re-pivot
		if (FMath::IsNearlyEqual(CurrentVelocityAccelerationDot, 1.0f, 0.01f))
			IsCharacterPivoting = false;
	}
	// Start pivoting if the dot product indicates a reversal
	else if (CurrentVelocityAccelerationDot < 0)
		IsCharacterPivoting = true;

	//	--------------------- CHECK FOR TURNING-IN-PLACE ---------------------

	if (!IsCharacterTurningInPlace)
		TurnInPlaceStartingYaw = OwningCharacter->GetActorRotation().Yaw;
	IsCharacterTurningInPlace = !IsCharacterMoving && FMath::Abs(CharacterYawDelta) > 0.1f;

	// Update the Lean angle of the character
	CalculateLeanAngle();

	// Update the locomotion state of the character to reach the target state
	HandleLocomotionStateChange();
}

void UFPCCharacterMovementComponent::HandleLocomotionStateChange()
{
	// Set the character to default locomotion target state if it's not moving
	// TODO: Allow the default state to be set somewhere globally
	if (!IsCharacterMoving && !IsCharacterAccelerating && TargetLocomotionState != ELocomotionState::Running)
		TargetLocomotionState = ELocomotionState::Running;

	//Check if the character is in a state where it can only walk.
	//For Example, when in Crouch state or ADS state or is armed but moving sideways or backwards etc
	bool canOnlyWalk = IsCrouching() || bIsCharacterInProneState || FPCCharacterWeaponManager->bIsCharacterInADSState || (FPCCharacterWeaponManager->bIsCharacterArmed && CurrentAccelerationDirection
		!= ELocomotionDirection::Forward);

	if (canOnlyWalk)
	{
		SetCurrentLocomotionStateWithSettings(IsCrouching() ? ELocomotionState::Crouching : ELocomotionState::Walking);
	}
	else if (currentLocomotionState != TargetLocomotionState)
	{
		bool canOnlyRun = !FPCCharacterWeaponManager->bIsCharacterArmed && CurrentVelocityDirection != ELocomotionDirection::Forward;
		SetCurrentLocomotionStateWithSettings(canOnlyRun ? ELocomotionState::Running : TargetLocomotionState);
	}
}

void UFPCCharacterMovementComponent::SetCurrentLocomotionState(ELocomotionState newLocomotionState)
{
	currentLocomotionState = newLocomotionState;
}

void UFPCCharacterMovementComponent::SetCurrentLocomotionStateWithSettings(ELocomotionState newLocomotionState)
{
	if (currentLocomotionState != newLocomotionState)
	{
		SetCurrentLocomotionState(newLocomotionState);
		SetLocomotionStateSettings(currentLocomotionState);
	}
}

void UFPCCharacterMovementComponent::SetCurrentVelocityDirection(ELocomotionDirection newVelocityDirection)
{
	if (CurrentVelocityDirection == newVelocityDirection)
		return;

	CurrentVelocityDirection = newVelocityDirection;
}

void UFPCCharacterMovementComponent::SetCurrentAccelerationDirection(ELocomotionDirection newAccelerationDirection)
{
	if (CurrentAccelerationDirection == newAccelerationDirection)
		return;

	CurrentAccelerationDirection = newAccelerationDirection;
}

void UFPCCharacterMovementComponent::SetLocomotionStateSettings(ELocomotionState newLocomotionState)
{
	const FLocomotionStateSetting& StateSettings = FPCCharacterData->LocomotionStateSettings[newLocomotionState];
	bUseSeparateBrakingFriction = StateSettings.bUseSeparateBrakingFriction;
	MaxWalkSpeed = StateSettings.MaxWalkSpeed;
	MaxWalkSpeedCrouched = StateSettings.MaxWalkSpeed;
	MaxAcceleration = StateSettings.MaxAcceleration;
	BrakingDecelerationWalking = StateSettings.BrakingDeceleration;
	BrakingFrictionFactor = StateSettings.BrakingFrictionFactor;
	BrakingFriction = StateSettings.BrakingFriction;
}

ELocomotionDirection UFPCCharacterMovementComponent::CalculateLocomotionDirection(const float LocomotionDirectionAngle, const ELocomotionDirection CurrentDirection) const
{
	// First check for deadzones

	//If current direction is forward and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Forward && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.X - DeadZone, ForwardLimits.Y + DeadZone))
		return ELocomotionDirection::Forward;
	//If current direction is backward and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Backward && (LocomotionDirectionAngle < BackwardLimits.X + DeadZone || LocomotionDirectionAngle > BackwardLimits.Y - DeadZone))
		return ELocomotionDirection::Backward;
	//If current direction is right and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Right && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.Y - DeadZone, BackwardLimits.Y + DeadZone))
		return ELocomotionDirection::Right;
	//If current direction is left and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Left && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, BackwardLimits.X - DeadZone, ForwardLimits.X + DeadZone))
		return ELocomotionDirection::Left;

	// If the above conditions weren't met, calculate the direction manually

	//Check for Forward range
	if (UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.X, ForwardLimits.Y))
		return ELocomotionDirection::Forward;

	// Check for Backward range
	if (LocomotionDirectionAngle < BackwardLimits.X || LocomotionDirectionAngle > BackwardLimits.Y)
		return ELocomotionDirection::Backward;

	//Check for right and left. We only need to test if it's positive angle since we've already ruled out forward and backward
	return LocomotionDirectionAngle > 0 ? ELocomotionDirection::Right : ELocomotionDirection::Left;
}

void UFPCCharacterMovementComponent::CalculateLeanAngle()
{
	// Get the yaw angular velocity and multiply by strength 
	LeanAngle = YawAngularVelocity * FPCCharacterData->TurningLeanStrength;

	// If the character is moving backwards, negate the lean angle
	if (CurrentVelocityDirection == ELocomotionDirection::Backward)
		LeanAngle *= -1;

	LeanAngle = FMath::Clamp(LeanAngle, -30.0f, 30.0f);
}
