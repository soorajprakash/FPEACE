// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "FPCEnemyCombatAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FPEACE_API UFPCEnemyCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS_BASIC(UFPCEnemyCombatAttributeSet,BaseMeleeDamage);
	
protected:

	/*
	 * The amount of damage dealt to the target character by each melee attack by the enemy
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData BaseMeleeDamage;
};
