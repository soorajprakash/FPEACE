// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FPCGameInstance.generated.h"

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

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UFPCCharacterData> CharacterData;

	static UFPCGameInstance* GetInstance(const UObject* WorldContextObject);

private:
	static UFPCGameInstance* GameInstance;
};
