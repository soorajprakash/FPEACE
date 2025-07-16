// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCCharacterAbilityBase.h"
#include "EnhancedInputComponent.h"
#include "Gameplay/Actor/FPCCharacter.h"
#include "Gameplay/Actor/FPCGameplayPlayerController.h"

// ------------- Definitions for static members -------------
TWeakObjectPtr<AFPCGameplayPlayerController> UFPCCharacterAbilityBase::FPCPlayerController = nullptr;
TWeakObjectPtr<UEnhancedInputComponent> UFPCCharacterAbilityBase::FPCPlayerInputComponent = nullptr;
TWeakObjectPtr<AFPCCharacter> UFPCCharacterAbilityBase::FPCCharacter = nullptr;
TWeakObjectPtr<UFPCOperatorData> UFPCCharacterAbilityBase::FPCCharacterData = nullptr;

UFPCCharacterAbilityBase::UFPCCharacterAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFPCCharacterAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (!FPCPlayerController.IsValid())
		FPCPlayerController = Cast<AFPCGameplayPlayerController>(GetOwningActorFromActorInfo()->GetInstigatorController());

	if (!FPCPlayerInputComponent.IsValid() && FPCPlayerController.IsValid())
		FPCPlayerInputComponent = Cast<UEnhancedInputComponent>(FPCPlayerController->InputComponent);

	if (!FPCCharacter.IsValid())
		FPCCharacter = Cast<AFPCCharacter>(GetOwningActorFromActorInfo());

	if (!FPCCharacterData.IsValid() && FPCCharacter.IsValid())
		FPCCharacterData = FPCCharacter->GetCharacterData();
}
