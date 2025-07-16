// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "FPCMovementAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FPEACE_API UFPCMovementAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData TargetMovementSpeed;

	ATTRIBUTE_ACCESSORS_BASIC(UFPCMovementAttributeSet, TargetMovementSpeed);

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
};
