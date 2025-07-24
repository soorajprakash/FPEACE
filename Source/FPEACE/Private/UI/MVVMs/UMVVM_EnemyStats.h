// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "MVVMViewModelBase.h"
#include "UMVVM_EnemyStats.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class FPEACE_API UUMVVM_EnemyStats : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercentage() const { return MaxHealth != 0 ? Health / MaxHealth : 0; }

	UFUNCTION(BlueprintCallable)
	void InitializeWithASC(UAbilitySystemComponent* InASC);

	//---------------------- SETTERS ----------------------
	void SetHealth(const float& NewHealth);
	void SetMaxHealth(const float& NewMaxHealth);

protected:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter)
	float Health = 100;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter)
	float MaxHealth = 100;

private:
	void UpdateHealthFromAttributes();

	void OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);

	UPROPERTY()
	UAbilitySystemComponent* ASC;
};
