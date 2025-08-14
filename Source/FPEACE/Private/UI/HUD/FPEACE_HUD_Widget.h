// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/FPCUserWidget.h"
#include "FPEACE_HUD_Widget.generated.h"

struct FOnAttributeChangeData;
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
	void OnPlayerHealthChanged(const float CurrentHealth);

private:
	UPROPERTY()
	TWeakObjectPtr<AFPCOperator> OwningOperator;

	void OnPlayerHealthChangedCallback(const FOnAttributeChangeData& OnAttributeChangeData);
};
