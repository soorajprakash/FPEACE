// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "FPCHealthAttributeSet.generated.h"

/**
 * Attribute Set for Health values
 */
UCLASS()
class FPEACE_API UFPCHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData Damage;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	ATTRIBUTE_ACCESSORS_BASIC(UFPCHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UFPCHealthAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS_BASIC(UFPCHealthAttributeSet, MaxHealth);
};
