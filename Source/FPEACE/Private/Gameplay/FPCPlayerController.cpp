// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCPlayerController.h"
#include "CommonEnums.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EngineExtensions/FPCGameInstance.h"
#include "InputMappingContext.h"
#include "DataStructures/FPCCharacterData.h"

void AFPCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Setup input mapping context
	if (UEnhancedInputLocalPlayerSubsystem* EILPS = CastChecked<UEnhancedInputLocalPlayerSubsystem>(ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())))
		if (!FPCCharacterInputMapping.IsNull())
		{
			EILPS->ClearAllMappings();
			EILPS->AddMappingContext(FPCCharacterInputMapping.LoadSynchronous(), 0);
		}

	//Set the initial Camera mode for local player
	SetCharacterCameraMode(UFPCGameInstance::GetInstance(this)->CharacterData->StartingCameraMode);
}

void AFPCPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

void AFPCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EInputComp = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EInputComp->BindAction(CameraSwitchAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCPlayerController::ToggleCameraMode);
	}
}

void AFPCPlayerController::SetCharacterCameraMode(ECameraMode newCameraMode)
{
	CurrentCameraMode = newCameraMode;
	OnCameraModeChanged.Broadcast(newCameraMode);
}

void AFPCPlayerController::ToggleCameraMode()
{
	ECameraMode newMode = CurrentCameraMode == ECameraMode::FPS ? ECameraMode::TPS : ECameraMode::FPS;
	SetCharacterCameraMode(newMode);
}
