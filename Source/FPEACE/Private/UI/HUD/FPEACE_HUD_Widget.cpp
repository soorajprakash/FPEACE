// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPEACE_HUD_Widget.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"


void UFPEACE_HUD_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	OwningOperator = Cast<AFPCOperator>(GetOwningPlayerPawn());

	if (OwningOperator.IsValid())
		OwningOperator.Get()->OnHealthChangedEvent.AddDynamic(
			this, &UFPEACE_HUD_Widget::OnPlayerHealthChangedCallback);
}

void UFPEACE_HUD_Widget::OnPlayerHealthChanged_Implementation(const float CurrentHealth, const ELivingCharacterHealthState CurrentHealthState)
{
}

void UFPEACE_HUD_Widget::OnPlayerHealthChangedCallback(const float CurrentHealth, const ELivingCharacterHealthState HealthState)
{
	OnPlayerHealthChanged(CurrentHealth, HealthState);
}
