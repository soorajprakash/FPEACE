// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void AFPCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* EILPS = CastChecked<UEnhancedInputLocalPlayerSubsystem>(ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())))
		if (!FPCCharacterInputMapping.IsNull())
		{
			EILPS->ClearAllMappings();
			EILPS->AddMappingContext(FPCCharacterInputMapping.LoadSynchronous(), 0);
		}
}
