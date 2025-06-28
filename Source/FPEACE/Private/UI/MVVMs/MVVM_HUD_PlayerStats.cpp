// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "UI/MVVMs/MVVM_HUD_PlayerStats.h"

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
