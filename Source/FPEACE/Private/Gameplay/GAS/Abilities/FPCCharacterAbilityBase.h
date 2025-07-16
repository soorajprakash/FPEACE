// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FPCCharacterAbilityBase.generated.h"

class UFPCOperatorData;
class AFPCCharacter;
class AFPCGameplayPlayerController;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class FPEACE_API UFPCCharacterAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFPCCharacterAbilityBase();

protected:
	static TWeakObjectPtr<AFPCGameplayPlayerController> FPCPlayerController;
	static TWeakObjectPtr<UEnhancedInputComponent> FPCPlayerInputComponent;
	static TWeakObjectPtr<AFPCCharacter> FPCCharacter;
	static TWeakObjectPtr<UFPCOperatorData> FPCCharacterData;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
