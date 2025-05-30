// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCGameModeBase.h"
#include "FPCSampleGameMode.generated.h"

class UFPCGameInstance;
/**
 * The game mode class that is used for one of the sample game modes to be included in the project
 */
UCLASS()
class FPEACE_API AFPCSampleGameMode : public AFPCGameModeBase
{
	GENERATED_BODY()

protected:
	
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};
