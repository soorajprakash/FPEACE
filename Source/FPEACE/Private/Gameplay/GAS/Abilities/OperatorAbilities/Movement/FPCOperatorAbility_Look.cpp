// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCOperatorAbility_Look.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/Actor/FPCGameplayPlayerController.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/Actor/Operator/Components/FPCFullScreenJoystickComponent.h"

void UFPCOperatorAbility_Look::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (FPCPlayerInputComponent.IsValid())
		InputBindingHandle = FPCPlayerInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UFPCOperatorAbility_Look::LookAround).GetHandle();

	if (FPCOperator.IsValid())
		FPCOperator->GetFPCFullScreenJoystickComp()->OnRightDelta.AddDynamic(this, &UFPCOperatorAbility_Look::OnDrag);
}

void UFPCOperatorAbility_Look::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          bool bReplicateEndAbility,
                                          bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (FPCPlayerInputComponent.IsValid())
		FPCPlayerInputComponent->RemoveBindingByHandle(InputBindingHandle);

	if (FPCOperator.IsValid())
		FPCOperator->GetFPCFullScreenJoystickComp()->OnRightDelta.RemoveAll(this);
}

void UFPCOperatorAbility_Look::OnDrag(FVector2D Delta)
{
	LookAround(FInputActionValue(Delta * 0.2f));
}

void UFPCOperatorAbility_Look::LookAround(const FInputActionValue& InputActionValue)
{
	if (!FPCPlayerController.IsValid())
		return;

	FVector2D input = InputActionValue.Get<FVector2D>();
	if (FPCOperatorData.IsValid())
	{
		FRotator ControlRotation = FPCPlayerController->GetControlRotation();

		// Add Yaw Input
		ControlRotation.Yaw += input.X;
		ControlRotation.Pitch = FRotator::NormalizeAxis(ControlRotation.Pitch); // Normalizing flips the direction.

		// Add Pitch Input
		ControlRotation.Pitch -= input.Y; // Subtracting instead of adding since we are normailizing the values in the next step.
		ControlRotation.Pitch = FRotator::NormalizeAxis(ControlRotation.Pitch); // Normalizing flips the direction.

		// Clamp the pitch between the limits defined in character data asset
		ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch, -FPCOperatorData->ControllerRotationPitchClamp, FPCOperatorData->ControllerRotationPitchClamp);

		// Set the updated control rotation
		FPCPlayerController->SetControlRotation(ControlRotation);
	}

	// FPCCameraManagerComp->UpdateCameraState();
}
