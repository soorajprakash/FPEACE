// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCAnimInstance.h"

#include "AnimationEditorUtils.h"
#include "CommonEnums.h"
#include "FPCCharacter.h"
#include "EngineExtensions/FPCSkeletalMeshComponent.h"
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
