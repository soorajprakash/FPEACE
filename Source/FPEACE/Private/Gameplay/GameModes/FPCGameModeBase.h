// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPCGameModeBase.generated.h"

class UFPCGameInstance;
class UFPCOperatorAnimInstance;

UCLASS()
class FPEACE_API AFPCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPCGameModeBase();

protected:
	UPROPERTY()
	TObjectPtr<UFPCGameInstance> FPCGameInstance;
};
