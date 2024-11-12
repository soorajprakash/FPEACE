// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCAnimInstance.h"

#include "CommonEnums.h"
#include "FPCCharacter.h"
#include "FPCCharacterMovementComponent.h"
#include "FPCGameInstance.h"
#include "DataStructures/FCameraModeAnimSelectionStruct.h"
#include "DataStructures/FPCCharacterData.h"

TSoftClassPtr<UFPCAnimInstance> UFPCAnimInstance::GetAnimClassFor(const UObject* WorldContextObject, ECameraMode targetCameraMode, FName animStateName, const FString& reasonForGettingThisAnim)
{
	TSoftClassPtr<UFPCAnimInstance> ReturnValue;
	if (UFPCGameInstance* GameInstance = UFPCGameInstance::GetInstance(WorldContextObject))
		if (UFPCCharacterData* CharacterData = GameInstance->CharacterData.Get())
			if (UDataTable* AnimClassTable = CharacterData->AnimClassTable.Get())
				if (FCameraModeAnimSelectionStruct* selectedItem = AnimClassTable->FindRow<FCameraModeAnimSelectionStruct>(animStateName, reasonForGettingThisAnim))
				{
					switch (targetCameraMode)
					{
					case ECameraMode::FPS:
						ReturnValue = selectedItem->FPSAnimationClass;
						break;

					case ECameraMode::TPS:
						ReturnValue = selectedItem->TPSAnimationClass;
						break;
					}
				}

	return ReturnValue;
}

void UFPCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (OwningCharacterMovementComponent == nullptr) if (AFPCCharacter* FPCCharcater = CastChecked<AFPCCharacter>(TryGetPawnOwner())) OwningCharacterMovementComponent = FPCCharcater->
		GetCharacterMovementComponent();

	if (MainAnimInstance == nullptr) MainAnimInstance = CastChecked<UFPCAnimInstance>(GetOwningComponent()->GetAnimInstance());
}

void UFPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwningCharacterMovementComponent)
	{
		CharacterVelocity = OwningCharacterMovementComponent->Velocity;
		CharacterSpeed = CharacterVelocity.Length() * CharacterVelocity.GetSafeNormal().Dot(GetOwningActor()->GetActorForwardVector());

		CharacterVelocity2D = FVector2D(CharacterVelocity.X, CharacterVelocity.Y);
		CharacterSpeed2D = CharacterVelocity2D.Length() * CharacterVelocity.GetSafeNormal().Dot(GetOwningActor()->GetActorForwardVector());
	}

	// Update transition rule values
	IsCharacterMoving = CharacterVelocity2D.Length() > 0.01f;
}
