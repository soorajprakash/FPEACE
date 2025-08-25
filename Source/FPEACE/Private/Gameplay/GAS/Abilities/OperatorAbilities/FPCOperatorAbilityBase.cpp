// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCOperatorAbilityBase.h"
#include "EnhancedInputComponent.h"
#include "Gameplay/Actor/FPCGameplayPlayerController.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"

// ------------- Definitions for static members -------------
TWeakObjectPtr<AFPCGameplayPlayerController> UFPCOperatorAbilityBase::FPCPlayerController = nullptr;
TWeakObjectPtr<UEnhancedInputComponent> UFPCOperatorAbilityBase::FPCPlayerInputComponent = nullptr;
TWeakObjectPtr<UFPCOperatorMovementComponent> UFPCOperatorAbilityBase::FPCOperatorMovementComponent = nullptr;
TWeakObjectPtr<AFPCOperator> UFPCOperatorAbilityBase::FPCOperator = nullptr;
TWeakObjectPtr<UFPCOperatorData> UFPCOperatorAbilityBase::FPCOperatorData = nullptr;

UFPCOperatorAbilityBase::UFPCOperatorAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFPCOperatorAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (!FPCPlayerController.IsValid())
		FPCPlayerController = Cast<AFPCGameplayPlayerController>(GetOwningActorFromActorInfo()->GetInstigatorController());

	if (!FPCPlayerInputComponent.IsValid() && FPCPlayerController.IsValid())
		FPCPlayerInputComponent = Cast<UEnhancedInputComponent>(FPCPlayerController->InputComponent);

	if (!FPCOperatorMovementComponent.IsValid() && FPCOperator.IsValid())
		FPCOperatorMovementComponent = FPCOperator->GetFPCMovementComp();

	if (!FPCOperator.IsValid())
		FPCOperator = Cast<AFPCOperator>(GetOwningActorFromActorInfo());

	if (!FPCOperatorData.IsValid() && FPCOperator.IsValid())
		FPCOperatorData = FPCOperator->GetOperatorData();
}
