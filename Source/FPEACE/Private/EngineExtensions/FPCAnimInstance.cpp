// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCAnimInstance.h"

#include "CommonEnums.h"
#include "FPCCharacter.h"
#include "FPCCharacterMovementComponent.h"
#include "FPCGameInstance.h"
#include "KismetAnimationLibrary.h"
#include "DataStructures/FCameraModeAnimSelectionStruct.h"
#include "DataStructures/FPCCharacterData.h"
#include "Kismet/KismetMathLibrary.h"

void UFPCAnimInstance::SetCurrentLocomotionState(ELocomotionState newLocomotionState)
{
	currentLocomotionState = newLocomotionState;
	currentLocomotionStateFloat = UKismetMathLibrary::Conv_ByteToDouble(static_cast<uint8>(currentLocomotionState));
}

TSoftClassPtr<UFPCAnimInstance> UFPCAnimInstance::GetAnimClassFor(const ECameraMode TargetCameraMode, const FName AnimStateName, const FString& ReasonForGettingThisAnim)
{
	TSoftClassPtr<UFPCAnimInstance> ReturnValue;
	if (GetCharacterData() != nullptr)
		if (const UDataTable* AnimClassTable = OwningCharacterData->AnimClassTable.Get())
			if (const FCameraModeAnimSelectionStruct* SelectedItem = AnimClassTable->FindRow<FCameraModeAnimSelectionStruct>(AnimStateName, ReasonForGettingThisAnim))
			{
				switch (TargetCameraMode)
				{
				case ECameraMode::FPS:
					ReturnValue = SelectedItem->FPSAnimationClass;
					break;

				case ECameraMode::TPS:
					ReturnValue = SelectedItem->TPSAnimationClass;
					break;
				}
			}

	return ReturnValue;
}

TObjectPtr<UFPCCharacterData> UFPCAnimInstance::GetCharacterData()
{
	if (OwningCharacterData == nullptr)
	{
		if (const UFPCGameInstance* GameInstance = UFPCGameInstance::GetInstance(this))
			OwningCharacterData = GameInstance->CharacterData;
	}

	return OwningCharacterData;
}

void UFPCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (OwningCharacter == nullptr)
		OwningCharacter = CastChecked<AFPCCharacter>(TryGetPawnOwner());

	if (OwningCharacter && OwningCharacterMovementComponent == nullptr)
		OwningCharacterMovementComponent = OwningCharacter->GetCharacterMovementComponent();

	if (!isBaseAnimInstance)
		return;

	if (GetCharacterData() != nullptr)
	{
		ForwardLimits = OwningCharacterData->CharacterDirectionLimits.ForwardLimits;
		BackwardLimits = OwningCharacterData->CharacterDirectionLimits.BackwardLimits;
		DeadZone = OwningCharacterData->CharacterDirectionLimits.DirectionalDeadzone;
	}
}

void UFPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!isBaseAnimInstance)
		return;

	if (OwningCharacterMovementComponent)
	{
		// Calculate Velocity
		CharacterVelocity = OwningCharacterMovementComponent->Velocity;
		CharacterAbsoluteSpeed = UKismetMathLibrary::VSizeXY(CharacterVelocity);

		CharacterVelocity2D = FVector(CharacterVelocity.X, CharacterVelocity.Y, 0);
		CharacterAbsoluteSpeed2D = UKismetMathLibrary::VSizeXY(CharacterVelocity2D);

		//Calculate the direction angle
		DirectionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterVelocity2D, OwningCharacter->GetActorRotation());
		CurrentLocomotionDirection = GetLocomotionDirection(DirectionAngle);

		//Update the Lean Angle
		CalculateLeanAngle(DeltaSeconds);
	}

	// Update transition rule values
	IsCharacterMoving = CharacterVelocity2D.Length() > 0.01f;
}

void UFPCAnimInstance::CalculateLeanAngle(float DeltaSeconds)
{
	float currentYaw = OwningCharacter->GetActorRotation().Yaw;

	// Get the yaw angular velocity and multiply by strength 
	LeanAngle = ((currentYaw - previousYaw) / DeltaSeconds) * OwningCharacterData->TurningLeanStrength;

	// If the character is moving backwards, negate the lean angle
	if (CurrentLocomotionDirection == ELocomotionDirection::Backward)
		LeanAngle *= -1;

	LeanAngle = FMath::Clamp(LeanAngle, -30.0f, 30.0f);
	previousYaw = currentYaw;

	UE_LOG(LogTemp, Warning, TEXT("LeanAngle: %f"), LeanAngle);
}


ELocomotionDirection UFPCAnimInstance::GetLocomotionDirection(const float LocomotionDirectionAngle) const
{
	// First check for deadzones

	//If current direction is forward and direction angle is within deadzone buffer, keep the direction
	if (CurrentLocomotionDirection == ELocomotionDirection::Forward && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.X - DeadZone, ForwardLimits.Y + DeadZone))
		return ELocomotionDirection::Forward;
	//If current direction is backward and direction angle is within deadzone buffer, keep the direction
	if (CurrentLocomotionDirection == ELocomotionDirection::Backward && (LocomotionDirectionAngle < BackwardLimits.X + DeadZone || LocomotionDirectionAngle > BackwardLimits.Y - DeadZone))
		return ELocomotionDirection::Backward;
	//If current direction is right and direction angle is within deadzone buffer, keep the direction
	if (CurrentLocomotionDirection == ELocomotionDirection::Right && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.Y - DeadZone, BackwardLimits.Y + DeadZone))
		return ELocomotionDirection::Right;
	//If current direction is left and direction angle is within deadzone buffer, keep the direction
	if (CurrentLocomotionDirection == ELocomotionDirection::Left && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, BackwardLimits.X - DeadZone, ForwardLimits.X + DeadZone))
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
