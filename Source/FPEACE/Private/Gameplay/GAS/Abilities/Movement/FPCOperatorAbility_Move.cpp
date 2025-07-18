// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCOperatorAbility_Move.h"
#include "EnhancedInputComponent.h"
#include "Gameplay/Actor/FPCGameplayPlayerController.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"

void UFPCOperatorAbility_Move::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (FPCPlayerInputComponent.IsValid())
		InputBindingHandle = FPCPlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UFPCOperatorAbility_Move::MoveAround).GetHandle();
}

void UFPCOperatorAbility_Move::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (FPCPlayerInputComponent.IsValid())
		FPCPlayerInputComponent->RemoveBindingByHandle(InputBindingHandle);
}

void UFPCOperatorAbility_Move::MoveAround(const FInputActionValue& InputActionValue)
{
	if (!FPCPlayerController.IsValid() || !FPCOperator.IsValid())
		return;

	FVector2D Input = InputActionValue.Get<FVector2D>();
	Input = Input.GetRotated(-FPCPlayerController->GetControlRotation().Yaw); // Rotate the input to face the character's direction
	FVector Input3D = FVector(Input.Y, Input.X, 0);
	FPCOperator->AddMovementInput(Input3D);
}
