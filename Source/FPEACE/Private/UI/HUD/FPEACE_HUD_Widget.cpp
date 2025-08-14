// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPEACE_HUD_Widget.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/GAS/AttribueSets/FPCHealthAttributeSet.h"

void UFPEACE_HUD_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	OwningOperator = Cast<AFPCOperator>(GetOwningPlayerPawn());

	if (OwningOperator.IsValid())
		OwningOperator.Get()->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UFPCHealthAttributeSet::GetHealthAttribute()).AddUObject(
			this, &UFPEACE_HUD_Widget::OnPlayerHealthChangedCallback);
}

void UFPEACE_HUD_Widget::OnPlayerHealthChanged_Implementation(const float CurrentHealth)
{
}

void UFPEACE_HUD_Widget::OnPlayerHealthChangedCallback(const FOnAttributeChangeData& OnAttributeChangeData)
{
	OnPlayerHealthChanged(OnAttributeChangeData.NewValue);
}
