// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "UI/MVVMs/MVVM_HUD_PlayerStats.h"
#include "Gameplay/GAS/AttribueSets/FPCHealthAttributeSet.h"

void UMVVM_HUD_PlayerStats::InitializeWithASC(UAbilitySystemComponent* InASC)
{
	if (!InASC) return;
	ASC = InASC;

	// Subscribe to the health-changed event
	ASC->GetGameplayAttributeValueChangeDelegate(
		UFPCHealthAttributeSet::GetHealthAttribute()
	).AddUObject(this, &UMVVM_HUD_PlayerStats::OnHealthChanged);
}

void UMVVM_HUD_PlayerStats::SetScore(const int32& NewScore)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Score, NewScore))
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Score);
}

void UMVVM_HUD_PlayerStats::SetHealth(const float& NewHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Health, NewHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Health);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercentage);
	}
}

void UMVVM_HUD_PlayerStats::SetMaxHealth(const float& NewMaxHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MaxHealth);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercentage);
	}
}

void UMVVM_HUD_PlayerStats::SetSelectedOperatorData(const FFPEACEOperatorData& NewOperatorData)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(SelectedOperatorData, NewOperatorData))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(SelectedOperatorData);
		OnOperatorDataChangedEvent.ExecuteIfBound();
	}
}

void UMVVM_HUD_PlayerStats::UpdateHealthFromAttributes()
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

void UMVVM_HUD_PlayerStats::OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	UpdateHealthFromAttributes();
}
