// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCGameModeBase.h"
#include "Gameplay/ExtendedClasses/FPCGameInstance.h"


AFPCGameModeBase::AFPCGameModeBase()
{
	if (!FPCGameInstance)
		if (UWorld* World = GetWorld())
			FPCGameInstance = Cast<UFPCGameInstance>(World->GetGameInstance());
}
