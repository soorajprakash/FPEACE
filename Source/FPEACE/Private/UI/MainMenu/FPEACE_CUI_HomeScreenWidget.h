// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonUI/Public/CommonActivatableWidget.h"
#include "FPEACE_CUI_HomeScreenWidget.generated.h"

class AFPCGameplayPlayerController;
/**
 * 
 */
UCLASS(Abstract)
class FPEACE_API UFPEACE_CUI_HomeScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	
	UFUNCTION(BlueprintCallable)
	void StartPlay();
};
