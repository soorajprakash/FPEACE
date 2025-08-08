// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonUI/Public/CommonButtonBase.h"
#include "DataStructures/FPEACEOperatorsDataMap.h"
#include "FPEACE_CUI_OperatorCardButton.generated.h"

class UFPCGameInstance;
class AFPCMainMenuLevelScriptActor;
/**
 * This class handles the button for the operator card in the operator selection screen
 */
UCLASS(Abstract)
class FPEACE_API UFPEACE_CUI_OperatorCardButton : public UCommonButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE")
	FFPEACEOperatorData CardOperatorData;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetupCard(const FFPEACEOperatorData& OperatorData);

	virtual void NativeOnSelected(bool bBroadcast) override;

	virtual void NativeOnDeselected(bool bBroadcast) override;

	virtual void NativePreConstruct() override;

private:
	UPROPERTY()
	TObjectPtr<AFPCMainMenuLevelScriptActor> MainMenuLevelScriptActor;

	UPROPERTY()
	TObjectPtr<UFPCGameInstance> FPCGameInstance;
};
