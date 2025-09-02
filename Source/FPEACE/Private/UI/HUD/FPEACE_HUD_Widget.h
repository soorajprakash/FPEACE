// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/FPCUserWidget.h"
#include "FPEACE_HUD_Widget.generated.h"

enum class ELivingCharacterHealthState : uint8;
class AFPCOperator;
/**
 * 
 */
UCLASS(Abstract)
class FPEACE_API UFPEACE_HUD_Widget : public UFPCUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnPlayerHealthChanged(const float CurrentHealth, const ELivingCharacterHealthState CurrentHealthState);

private:
	UPROPERTY()
	TWeakObjectPtr<AFPCOperator> OwningOperator;

	UFUNCTION()
	void OnPlayerHealthChangedCallback(const float CurrentHealth, const ELivingCharacterHealthState HealthState);
};
