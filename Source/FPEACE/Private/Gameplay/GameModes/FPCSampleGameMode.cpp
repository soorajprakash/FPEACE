// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCSampleGameMode.h"
#include "Gameplay/FPCGameInstance.h"
#include "Gameplay/Character/FPCCharacter.h"

UClass* AFPCSampleGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// Set the selected operator class as the default pawn class for this game mode
	if (FPCGameInstance)
		return FPCGameInstance->SelectedOperatorPawnClass;

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}
