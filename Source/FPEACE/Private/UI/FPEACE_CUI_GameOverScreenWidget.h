// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "FPEACE_CUI_GameOverScreenWidget.generated.h"

/**
 * The Pause Menu Screen widget class
 */
UCLASS(Abstract)
class FPEACE_API UFPEACE_CUI_GameOverScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetSurvivalTimeText(float Seconds) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetScoreText(int32 Score) const;
};
