// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCAnimInstance.h"

#include "CommonEnums.h"
#include "FPCCharacter.h"
#include "FPCGameInstance.h"
#include "DataStructures/FCameraModeAnimSelectionStruct.h"
#include "DataStructures/FPCCharacterData.h"

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
	LeanAngle = ((currentYaw - previousYaw) / DeltaSeconds) * GetCharacterData()->TurningLeanStrength;

	// If the character is moving backwards, negate the lean angle
	if (OwningCharacter->GetCurrentLocomotionDirection() == ELocomotionDirection::Backward)
		LeanAngle *= -1;

	LeanAngle = FMath::Clamp(LeanAngle, -30.0f, 30.0f);
	previousYaw = currentYaw;
}



