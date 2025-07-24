// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/FPCUserWidget.h"
#include "FPEACE_HUD_ScoreWidget.generated.h"

class UMVVM_HUD_PlayerStats;
class AFPCPlayerState;
/**
 * 
 */
UCLASS()
class FPEACE_API UFPEACE_HUD_ScoreWidget : public UFPCUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnScoreChanged(int32 Score);

private:
	UPROPERTY(EditDefaultsOnly)
	FName ViewModelName = FName("WidgetVM");

	UPROPERTY()
	TObjectPtr<UMVVM_HUD_PlayerStats> WidgetViewModelInstance;

	UPROPERTY()
	TWeakObjectPtr<AFPCPlayerState> FPCPlayerState;

	UFUNCTION()
	void HandleScoreChanged(int32 Score);
};
