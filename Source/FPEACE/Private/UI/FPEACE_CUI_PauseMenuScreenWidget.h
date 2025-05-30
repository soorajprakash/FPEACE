// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "FPEACE_CUI_PauseMenuScreenWidget.generated.h"

/**
 * The Pause Menu Screen widget class
 */
UCLASS()
class FPEACE_API UFPEACE_CUI_PauseMenuScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void TogglePauseMenu(bool bIsPaused);
};
