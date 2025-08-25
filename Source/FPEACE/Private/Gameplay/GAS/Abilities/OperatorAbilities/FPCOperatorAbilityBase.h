// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FPCOperatorAbilityBase.generated.h"

class UFPCOperatorMovementComponent;
class UEnhancedInputComponent;
class AFPCOperator;
class UFPCOperatorData;
class AFPCGameplayPlayerController;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class FPEACE_API UFPCOperatorAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFPCOperatorAbilityBase();

protected:
	static TWeakObjectPtr<AFPCGameplayPlayerController> FPCPlayerController;
	static TWeakObjectPtr<UEnhancedInputComponent> FPCPlayerInputComponent;
	static TWeakObjectPtr<UFPCOperatorMovementComponent> FPCOperatorMovementComponent;
	static TWeakObjectPtr<AFPCOperator> FPCOperator;
	static TWeakObjectPtr<UFPCOperatorData> FPCOperatorData;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
