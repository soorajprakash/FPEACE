// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/Abilities/OperatorAbilities/FPCOperatorAbilityBase.h"
#include "FPCOperatorAbility_Sprint.generated.h"

/**
 * Ability to use Tactical Sprint
 */
UCLASS()
class FPEACE_API UFPCOperatorAbility_Sprint : public UFPCOperatorAbilityBase
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
