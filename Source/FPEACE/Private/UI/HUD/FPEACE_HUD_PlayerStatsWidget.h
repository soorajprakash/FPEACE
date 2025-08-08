// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/FPCUserWidget.h"
#include "FPEACE_HUD_PlayerStatsWidget.generated.h"

class UMVVM_HUD_PlayerStats;
class UFPCOperatorWeaponManagerComponent;
class AFPCCharacter;
class UFPCGameInstance;
DECLARE_DYNAMIC_DELEGATE(FOnOperatorDataChanged);

/**
 * The base class for the player stats widget
 * Mostly used in the HUD
 */
UCLASS(Abstract)
class FPEACE_API UFPEACE_HUD_PlayerStatsWidget : public UFPCUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnOperatorDataChanged();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	FName ViewModelName = FName("WidgetVM");

	UPROPERTY()
	TObjectPtr<UMVVM_HUD_PlayerStats> WidgetViewModelInstance;

	UPROPERTY()
	TWeakObjectPtr<UFPCGameInstance> FPCGameInstance;
};
