// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCHealthAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Gameplay/Common/FPCGameplayTags.h"

void UFPCHealthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float CurrentHealth = GetHealth();
		float NewHealthValue = GetHealth() - GetDamage();
		NewHealthValue = FMath::Clamp(NewHealthValue, 0, GetMaxHealth());

		if (CurrentHealth != NewHealthValue)
		{
			SetHealth(NewHealthValue);
			Data.Target.RemoveLooseGameplayTag(Character_Health_AtMaxHealth);
		}

		SetDamage(0);
	}
	else if (Data.EvaluatedData.Attribute == GetHealAttribute())
	{
		float CurrentHealth = GetHealth();
		float NewHealthValue = GetHealth() + GetHeal();
		float MaxHealthValue = GetMaxHealth();
		NewHealthValue = FMath::Clamp(NewHealthValue, 0, MaxHealthValue);

		if (CurrentHealth != NewHealthValue)
			SetHealth(NewHealthValue);

		if (CurrentHealth == MaxHealthValue)
			Data.Target.AddLooseGameplayTag(Character_Health_AtMaxHealth);

		SetHeal(0);
	}
}
