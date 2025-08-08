// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "UMVVM_EnemyStats.h"
#include "Gameplay/GAS/AttribueSets/FPCHealthAttributeSet.h"

void UUMVVM_EnemyStats::InitializeWithASC(UAbilitySystemComponent* InASC)
{
	if (!InASC) return;
	ASC = InASC;

	// Subscribe to the health-changed event
	ASC->GetGameplayAttributeValueChangeDelegate(
		UFPCHealthAttributeSet::GetHealthAttribute()
	).AddUObject(this, &UUMVVM_EnemyStats::OnHealthChanged);
}

void UUMVVM_EnemyStats::SetHealth(const float& NewHealth)
{
	if (Health == NewHealth)
		return;

	if (UE_MVVM_SET_PROPERTY_VALUE(Health, NewHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Health);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercentage);
	}
}

void UUMVVM_EnemyStats::SetMaxHealth(const float& NewMaxHealth)
{
	if (MaxHealth == NewMaxHealth)
		return;

	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MaxHealth);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercentage);
	}
}

void UUMVVM_EnemyStats::UpdateHealthFromAttributes()
{
	float CurrentHealth = ASC->GetNumericAttribute(
		UFPCHealthAttributeSet::GetHealthAttribute()
	);

	float CurrentMaxHealth = ASC->GetNumericAttribute(
		UFPCHealthAttributeSet::GetMaxHealthAttribute()
	);

	SetHealth(CurrentHealth);
	SetMaxHealth(CurrentMaxHealth);
}

void UUMVVM_EnemyStats::OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	UpdateHealthFromAttributes();
}
