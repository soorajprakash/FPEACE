// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/Abilities/OperatorAbilities/FPCOperatorAbilityBase.h"
#include "FPCOperatorAbility_Look.generated.h"

struct FInputActionValue;
class UInputAction;
/**
 * Ability to look around
 */
UCLASS(Abstract, Blueprintable)
class FPEACE_API UFPCOperatorAbility_Look : public UFPCOperatorAbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Inputs")
	TObjectPtr<UInputAction> LookAction;

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

	UFUNCTION()
	void OnDrag(FVector2D Delta);
	
	// ----------------- INPUT BINDING FUNCTIONS ------------------
	void LookAround(const FInputActionValue& InputActionValue);
};
