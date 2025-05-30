// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCGameModeBase.h"
#include "Gameplay/FPCGameInstance.h"


AFPCGameModeBase::AFPCGameModeBase()
{
	if (!FPCGameInstance)
		if (UWorld* World = GetWorld())
			FPCGameInstance = Cast<UFPCGameInstance>(World->GetGameInstance());
}