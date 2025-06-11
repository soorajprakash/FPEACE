// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "DataStructures/FPEACEOperatorsDataMap.h"
#include "Engine/GameInstance.h"
#include "FPCGameInstance.generated.h"

class UFPEACE_CUI_PauseMenuScreenWidget;
class UFPCCharacterData;
/**
 * 
 */
UCLASS()
class FPEACE_API UFPCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual ~UFPCGameInstance() override;

	UPROPERTY(EditDefaultsOnly, Category="FPEACE")
	TSoftObjectPtr<UFPCCharacterData> CharacterData;

	UPROPERTY(VisibleAnywhere, Category="FPEACE")
	TSubclassOf<APawn> SelectedOperatorPawnClass;

	UPROPERTY(VisibleAnywhere, Category="FPEACE")
	FFPEACEOperatorData SelectedOperatorData;

	UPROPERTY(EditDefaultsOnly, Category="FPEACE")
	TSubclassOf<UFPEACE_CUI_PauseMenuScreenWidget> PauseMenuScreenWidgetClass;

	static UFPCGameInstance* GetInstance(const UObject* WorldContextObject);

private:
	static UFPCGameInstance* GameInstance;
};
