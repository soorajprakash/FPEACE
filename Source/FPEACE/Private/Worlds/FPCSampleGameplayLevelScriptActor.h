// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "FPCSampleGameplayLevelScriptActor.generated.h"

class UFPEACE_CUI_PauseMenuScreenWidget;
class AFPCGameplayPlayerController;
struct FInputActionValue;
class UFPCGameInstance;
class UInputAction;

UCLASS()
class FPEACE_API AFPCSampleGameplayLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPCSampleGameplayLevelScriptActor();

};
