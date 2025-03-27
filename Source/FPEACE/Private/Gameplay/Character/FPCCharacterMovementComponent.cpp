// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCCharacterMovementComponent.h"

#include "FPCCapsuleComponent.h"
#include "FPCCharacter.h"
#include "FPCCharacterWeaponManagerComponent.h"
#include "KismetAnimationLibrary.h"
#include "DataStructures/FPCCharacterData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
	if (IsCharacterAccelerating)
		SetTargetLocomotionState(TargetLocomotionState != ELocomotionState::Running ? ELocomotionState::Running : ELocomotionState::Sprinting);
}

void UFPCCharacterMovementComponent::ToggleCrouch()
{
	bWantsToCrouch = !bWantsToCrouch;
	currentLocomotionStance = bWantsToCrouch ? ELocomotionStance::Crouching : ELocomotionStance::Standing;
}

void UFPCCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (!OwningCharacter)
		OwningCharacter = Cast<AFPCCharacter>(GetOwner());

	if (OwningCharacter)
	{
		FPCCharacterData = OwningCharacter->GetCharacterData();
		OwningCharacterCapsule = OwningCharacter->GetFPCCapsuleComp();
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
	CharacterVelocityZ = Velocity.Z;
	CharacterAbsoluteSpeed = Velocity.Length();
	CharacterAbsoluteSpeed2D = UKismetMathLibrary::VSizeXY(CharacterVelocity2D);
	CharacterNormalizedSpeed = CharacterAbsoluteSpeed / MaxWalkSpeed;

	// Update if the character is moving
	if (IsCharacterMoving && CharacterVelocity2D.Length() < 1)
		IsCharacterMoving = false;
	else if (!IsCharacterMoving && CharacterVelocity2D.Length() > 10)
		IsCharacterMoving = true;

	IsCharacterAccelerating = !CharacterAcceleration.IsNearlyZero();

	//Calculate the directions
	if (IsCharacterAccelerating)
	{
		AccelerationDirectionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterAcceleration2D, OwningCharacter->GetActorRotation());
		SetCurrentAccelerationDirection(CalculateLocomotionDirection(AccelerationDirectionAngle, CurrentAccelerationDirection));
	}
	else
	{
		AccelerationDirectionAngle = 0;
		SetCurrentAccelerationDirection(ELocomotionDirection::Center);
	}

	if (!FMath::IsNearlyZero(CharacterVelocity2D.Size()))
	{
		VelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterVelocity2D, OwningCharacter->GetActorRotation());
		SetCurrentVelocityDirection(CalculateLocomotionDirection(VelocityDirectionAngle, CurrentVelocityDirection));

		// if (currentLocomotionState != ELocomotionState::Stationary)
		UpdateDirectionalMaxWalkSpeed(CurrentVelocityDirection);
	}
	else
	{
		VelocityDirectionAngle = 0;
		SetCurrentVelocityDirection(ELocomotionDirection::Center);
	}

	// Calculate the current delta distance covered by the character
	FVector CurrentWorldLocation = GetActorLocation();
	CurrentDeltaDistance = FVector::Distance(LastWorldLocation, CurrentWorldLocation);
	CurrentDeltaDistanceZ = CurrentWorldLocation.Z - LastWorldLocation.Z;
	LastWorldLocation = CurrentWorldLocation;

	// Precompute normalized velocity and acceleration for clarity
	const FVector NormalizedVelocity = CharacterVelocity2D.GetSafeNormal();
	const FVector NormalizedAcceleration = CharacterAcceleration2D.GetSafeNormal();

	// Calculate the dot product once
	CurrentVelocityAccelerationDot = FVector::DotProduct(NormalizedVelocity, NormalizedAcceleration);

	// Calculate Yaw Angular Velocity
	FRotator CurrentRotation = OwningCharacter->GetActorRotation();
	CharacterYawDelta = CurrentRotation.Yaw - PrevRotation.Yaw;
	YawAngularVelocity = CharacterYawDelta / DeltaTime;
	PrevRotation = CurrentRotation;

	// Calculate the character's distance to ground if in falling state
	if (IsFalling())
	{
		FVector TraceStart = OwningCharacter->GetActorLocation() - OwningCharacterCapsule->GetScaledCapsuleHalfHeight();
		FVector TraceEnd = TraceStart - FVector(0, 0, 10000);
		FHitResult HitResult;
		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStart, TraceEnd, OwningCharacterCapsule->GetScaledCapsuleRadius(),
		                                            UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>{}, EDrawDebugTrace::None, HitResult, true))
			DistanceToGroundWhileInAir = HitResult.Distance;
	}

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
	IsCharacterTurningInPlace = !IsCharacterMoving && FMath::Abs(CharacterYawDelta) > 0.5f;

	// Update the Lean angle of the character
	CalculateLeanAngle();

	// Update the locomotion state of the character to reach the target state
	HandleLocomotionStateChange();

	IsCharacterSlowingDown = LastFrameMaxSpeed > (IsCrouching() ? MaxWalkSpeedCrouched : MaxWalkSpeed);
	LastFrameMaxSpeed = IsCrouching() ? MaxWalkSpeedCrouched : MaxWalkSpeed;

	UpdateStateChanges();
}

void UFPCCharacterMovementComponent::HandleLocomotionStateChange()
{
	// Set the character to default locomotion target state if it's not moving
	// TODO: Allow the default state to be set somewhere globally
	if (!IsCharacterAccelerating)
	{
		SetCurrentLocomotionStateWithSettings(ELocomotionState::Stationary);
		SetTargetLocomotionState(ELocomotionState::Running);
	}
	else
	{
		//Check if the character is in a state where it can only walk.
		//For Example, when in Crouch state or ADS state or is armed but moving sideways or backwards etc
		bool canOnlyWalk = IsCrouching() || bIsCharacterInProneState || FPCCharacterWeaponManager->GetWantsToAds() || (FPCCharacterWeaponManager->GetIsCharacterArmed() &&
			CurrentAccelerationDirection
			!= ELocomotionDirection::Forward);

		if (canOnlyWalk)
		{
			SetCurrentLocomotionStateWithSettings(ELocomotionState::Walking);
		}
		else if (currentLocomotionState != TargetLocomotionState)
		{
			bool canOnlyRun = !FPCCharacterWeaponManager->GetIsCharacterArmed() && CurrentVelocityDirection != ELocomotionDirection::Forward;
			SetCurrentLocomotionStateWithSettings(canOnlyRun ? ELocomotionState::Running : TargetLocomotionState);
		}
	}
}

void UFPCCharacterMovementComponent::SetCurrentLocomotionState(ELocomotionState newLocomotionState)
{
	if (newLocomotionState == currentLocomotionState)
		return;

	currentLocomotionState = newLocomotionState;

	OnCurrentLocomotionStateChanged.Broadcast(currentLocomotionState);
}

void UFPCCharacterMovementComponent::SetTargetLocomotionState(ELocomotionState newLocomotionState)
{
	if (newLocomotionState == TargetLocomotionState)
		return;

	TargetLocomotionState = newLocomotionState;

	OnTargetLocomotionStateChanged.Broadcast(TargetLocomotionState);
}

void UFPCCharacterMovementComponent::SetCurrentLocomotionStateWithSettings(ELocomotionState newLocomotionState)
{
	if (currentLocomotionState != newLocomotionState)
	{
		SetCurrentLocomotionState(newLocomotionState);

		// if (currentLocomotionState != ELocomotionState::Stationary)
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
	const FLocomotionStateSetting& StateSettings = FPCCharacterData->LocomotionStanceSettings[currentLocomotionStance].LocomotionStateSettings[newLocomotionState];
	bUseSeparateBrakingFriction = StateSettings.bUseSeparateBrakingFriction;
	MaxWalkSpeed = StateSettings.MaxWalkSpeedForward;
	MaxWalkSpeedCrouched = StateSettings.MaxWalkSpeedForward;
	MaxAcceleration = StateSettings.MaxAcceleration;
	BrakingDecelerationWalking = StateSettings.BrakingDeceleration;
	BrakingFrictionFactor = StateSettings.BrakingFrictionFactor;
	BrakingFriction = StateSettings.BrakingFriction;
}

void UFPCCharacterMovementComponent::UpdateDirectionalMaxWalkSpeed(ELocomotionDirection newVelocityDirection)
{
	const FLocomotionStateSetting& StateSettings = FPCCharacterData->LocomotionStanceSettings[currentLocomotionStance].LocomotionStateSettings[currentLocomotionState];
	float maxDirectionalWalkSpeed = 0;
	switch (newVelocityDirection)
	{
	case ELocomotionDirection::Forward:
		maxDirectionalWalkSpeed = StateSettings.MaxWalkSpeedForward;
		break;

	case ELocomotionDirection::Backward:
		maxDirectionalWalkSpeed = StateSettings.MaxWalkSpeedBackward;
		break;

	case ELocomotionDirection::Right:
		maxDirectionalWalkSpeed = StateSettings.MaxWalkSpeedRight;
		break;

	case ELocomotionDirection::Left:
		maxDirectionalWalkSpeed = StateSettings.MaxWalkSpeedLeft;
		break;
	default: ;
	}

	if (currentLocomotionStance == ELocomotionStance::Crouching)
		MaxWalkSpeedCrouched = maxDirectionalWalkSpeed;
	else
		MaxWalkSpeed = maxDirectionalWalkSpeed;
}

void UFPCCharacterMovementComponent::UpdateStateChanges()
{
	// Check if the character changed it's moving state
	MovementStateChanged = IsCharacterMoving != WasMovingLastFrame;
	if (MovementStateChanged)
		PrevMovementState = WasMovingLastFrame;
	WasMovingLastFrame = IsCharacterMoving;

	// Check if the character changed its acceleration state
	AccelerationStateChanged = IsCharacterAccelerating != WasAcceleratingLastFrame;
	if (AccelerationStateChanged)
		PrevAccelerationState = WasAcceleratingLastFrame;
	WasAcceleratingLastFrame = IsCharacterAccelerating;

	// Check if Character's Current Locomotion State Changed
	CurrentLocomotionStateChanged = currentLocomotionState != LastFrameLocomotionState;
	if (CurrentLocomotionStateChanged)
		PrevLocomotionState = LastFrameLocomotionState;
	LastFrameLocomotionState = currentLocomotionState;

	// Check if Character's Current Locomotion Stance Changed
	CurrentLocomotionStanceChanged = currentLocomotionStance != LastFrameLocomotionStance;
	if (CurrentLocomotionStanceChanged)
		PrevLocomotionStance = LastFrameLocomotionStance;
	LastFrameLocomotionStance = currentLocomotionStance;

	// Check if Character's Target Locomotion State Changed
	TargetLocomotionStateChanged = TargetLocomotionState != LastFrameTargetLocomotionState;
	if (TargetLocomotionStateChanged)
		PrevTargetLocomotionState = LastFrameTargetLocomotionState;
	LastFrameTargetLocomotionState = TargetLocomotionState;

	// Check if Character's Velocity Direction Changed
	VelocityDirectionChanged = CurrentVelocityDirection != LastFrameVelocityDirection;
	if (VelocityDirectionChanged)
		PrevVelocityDirection = LastFrameVelocityDirection;
	LastFrameVelocityDirection = CurrentVelocityDirection;

	// Check if Character's Acceleration Direction Changed
	AccelerationDirectionChanged = CurrentAccelerationDirection != LastFrameAccelerationDirection;
	if (AccelerationDirectionChanged)
		PrevAccelerationDirection = LastFrameAccelerationDirection;
	LastFrameAccelerationDirection = CurrentAccelerationDirection;

	// If any of the preceding states changed, this will be true for that frame
	AnyMovementStateChanged = MovementStateChanged || AccelerationStateChanged || VelocityDirectionChanged ||
		AccelerationDirectionChanged || CurrentLocomotionStateChanged || CurrentLocomotionStanceChanged || TargetLocomotionStateChanged;
}

ELocomotionDirection UFPCCharacterMovementComponent::CalculateLocomotionDirection(const float LocomotionDirectionAngle, const ELocomotionDirection CurrentDirection) const
{
	// First check for dead-zones

	//If the current direction is forward and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Forward && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.X - DeadZone, ForwardLimits.Y + DeadZone))
		return ELocomotionDirection::Forward;
	//If the current direction is backward and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Backward && (LocomotionDirectionAngle < BackwardLimits.X + DeadZone || LocomotionDirectionAngle > BackwardLimits.Y - DeadZone))
		return ELocomotionDirection::Backward;
	//If the current direction is right and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Right && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.Y - DeadZone, BackwardLimits.Y + DeadZone))
		return ELocomotionDirection::Right;
	//If the current direction is left and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Left && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, BackwardLimits.X - DeadZone, ForwardLimits.X + DeadZone))
		return ELocomotionDirection::Left;

	// If the preceding conditions weren't met, calculate the direction manually

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
