// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCCharacterAbility_Move.h"
#include "EnhancedInputComponent.h"
#include "Gameplay/Actor/FPCCharacter.h"
#include "Gameplay/Actor/FPCGameplayPlayerController.h"

void UFPCCharacterAbility_Move::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (FPCPlayerInputComponent.IsValid())
		InputBindingHandle = FPCPlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UFPCCharacterAbility_Move::MoveAround).GetHandle();
}

void UFPCCharacterAbility_Move::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (FPCPlayerInputComponent.IsValid())
		FPCPlayerInputComponent->RemoveBindingByHandle(InputBindingHandle);
}

void UFPCCharacterAbility_Move::MoveAround(const FInputActionValue& InputActionValue)
{
	if (!FPCPlayerController.IsValid() || !FPCCharacter.IsValid())
		return;

	FVector2D Input = InputActionValue.Get<FVector2D>();
	Input = Input.GetRotated(-FPCPlayerController->GetControlRotation().Yaw); // Rotate the input to face the character's direction
	FVector Input3D = FVector(Input.Y, Input.X, 0);
	FPCCharacter->AddMovementInput(Input3D);
}
