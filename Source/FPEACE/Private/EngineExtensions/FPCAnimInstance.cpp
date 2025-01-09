// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCAnimInstance.h"

#include "CommonEnums.h"
#include "FPCCharacter.h"
#include "FPCSkeletalMeshComponent.h"
#include "DataStructures/FCameraModeAnimSelectionStruct.h"
#include "DataStructures/FPCCharacterData.h"

TSoftClassPtr<UFPCAnimInstance> UFPCAnimInstance::GetAnimClassFor(const ECameraMode TargetCameraMode, const FName AnimStateName, const FString& ReasonForGettingThisAnim)
{
	InitializeReferences();
	TSoftClassPtr<UFPCAnimInstance> ReturnValue;
	if (OwningCharacter->GetCharacterData() != nullptr)
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

void UFPCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	InitializeReferences();

	// Subscribe to character events
	OwningCharacter->OnLocomotionStateChanged.AddDynamic(this, &UFPCAnimInstance::OnCharacterLocomotionStateChanged);
}

void UFPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!isBaseAnimInstance)
		return;

	if (OwningCharacterMovementComponent)
	{
		//Update the Lean Angle
		CalculateLeanAngle(DeltaSeconds);
	}

	// Check for character pivoting
	if (OwningCharacter)
	{
		TriggerCharacterRePivot = false;

		// Precompute normalized velocity and acceleration for clarity
		const FVector NormalizedVelocity = OwningCharacter->GetCharacterVelocity2D().GetSafeNormal();
		const FVector NormalizedAcceleration = OwningCharacter->GetCharacterAcceleration2D().GetSafeNormal();

		// Calculate the dot product once
		const float VelocityAccelerationDot = FVector::DotProduct(NormalizedVelocity, NormalizedAcceleration);

		if (IsCharacterPivoting)
		{
			// Check if the character has stopped pivoting or needs to trigger a re-pivot
			if (FMath::IsNearlyEqual(VelocityAccelerationDot, 1.0f, 0.01f))
				IsCharacterPivoting = false;
			else if (FMath::Sign(PrevVelocityAccelerationDot) != FMath::Sign(VelocityAccelerationDot))
				TriggerCharacterRePivot = true;
		}
		// Start pivoting if the dot product indicates a reversal
		else if (VelocityAccelerationDot < 0 && !FMath::IsNearlyZero(VelocityAccelerationDot))
			IsCharacterPivoting = true;

		// Store the current dot product for future comparisons
		PrevVelocityAccelerationDot = VelocityAccelerationDot;
	}
}

void UFPCAnimInstance::CalculateLeanAngle(float DeltaSeconds)
{
	float currentYaw = OwningCharacter->GetActorRotation().Yaw;

	// Get the yaw angular velocity and multiply by strength 
	LeanAngle = ((currentYaw - previousYaw) / DeltaSeconds) * OwningCharacterData->TurningLeanStrength;

	// If the character is moving backwards, negate the lean angle
	if (OwningCharacter->GetCurrentLocomotionDirection() == ELocomotionDirection::Backward)
		LeanAngle *= -1;

	LeanAngle = FMath::Clamp(LeanAngle, -30.0f, 30.0f);
	previousYaw = currentYaw;
}

void UFPCAnimInstance::InitializeReferences()
{
	if (OwningCharacter == nullptr)
		OwningCharacter = CastChecked<AFPCCharacter>(TryGetPawnOwner());

	if (OwningMesh == nullptr)
		OwningMesh = CastChecked<UFPCSkeletalMeshComponent>(GetOwningComponent());

	if (OwningAnimInstance == nullptr)
		OwningAnimInstance = CastChecked<UFPCAnimInstance>(OwningMesh->GetAnimInstance());

	if (OwningCharacter && OwningCharacterMovementComponent == nullptr)
		OwningCharacterMovementComponent = OwningCharacter->GetCharacterMovementComponent();

	if (OwningCharacter)
		OwningCharacterData = OwningCharacter->GetCharacterData();
}
