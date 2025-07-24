// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCHealthAttributeSet.h"
#include "GameplayEffectExtension.h"

void UFPCHealthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float CurrentHealth = GetHealth();
		float NewHealthValue = GetHealth() - GetDamage();
		NewHealthValue = FMath::Clamp(NewHealthValue, 0, GetMaxHealth());

		if (CurrentHealth != NewHealthValue)
			SetHealth(NewHealthValue);

		SetDamage(0);
	}
}
