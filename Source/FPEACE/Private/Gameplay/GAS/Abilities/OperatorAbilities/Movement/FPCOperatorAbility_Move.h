// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Gameplay/GAS/Abilities/OperatorAbilities/FPCOperatorAbilityBase.h"
#include "FPCOperatorAbility_Move.generated.h"

class UInputAction;
/**
 * Ability to Walk
 */
UCLASS(Abstract, Blueprintable)
class FPEACE_API UFPCOperatorAbility_Move : public UFPCOperatorAbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TObjectPtr<UInputAction> MoveAction;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

private:
	/*
	 * Reference to the created input binding
	 */
	uint32 InputBindingHandle;

	// ----------------- INPUT BINDING FUNCTIONS ------------------
	void MoveAround(const FInputActionValue& InputActionValue);
};
