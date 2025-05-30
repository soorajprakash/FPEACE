// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCGameplayPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Gameplay/FPCGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/FPEACE_CUI_PauseMenuScreenWidget.h"
#include "Worlds/FPCSampleGameplayLevelScriptActor.h"


void AFPCGameplayPlayerController::TogglePauseMenu()
{
	// Pause game and open pause menu
	if (!bIsGamePaused)
	{
		PauseMenuScreenWidget->AddToViewport();
		SetInputMode(FInputModeUIOnly());
	}
	// Resume game and close pause menu
	else
	{
		PauseMenuScreenWidget->RemoveFromParent();
		SetInputMode(FInputModeGameOnly());
	}

	bIsGamePaused = !bIsGamePaused;
	UGameplayStatics::SetGamePaused(GetWorld(), bIsGamePaused);
	SetShowMouseCursor(bIsGamePaused);
	PauseMenuScreenWidget->TogglePauseMenu(bIsGamePaused);
}

void AFPCGameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* EILPS = CastChecked<UEnhancedInputLocalPlayerSubsystem>(ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())))
	{
		// Add Character Input Mapping Context. This contains character gameplay input mappings
		if (!FPCCharacterInputMapping.IsNull())
		{
			EILPS->ClearAllMappings();
			EILPS->AddMappingContext(FPCCharacterInputMapping.LoadSynchronous(), 0);
		}

		// Add Global Input Mapping Context. This contains "Escape" button binding etc
		if (!FPCGlobalInputMapping.IsNull())
			EILPS->AddMappingContext(FPCGlobalInputMapping.LoadSynchronous(), 0);
	}

	// Bind Global Inputs

	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent))
		EIC->BindAction(PauseMenuToggleAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCGameplayPlayerController::TogglePauseMenu);
}

void AFPCGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!FPCGameInstance)
		if (UWorld* World = GetWorld())
			FPCGameInstance = Cast<UFPCGameInstance>(World->GetGameInstance());

	if (!PauseMenuScreenWidget)
		PauseMenuScreenWidget = CreateWidget<UFPEACE_CUI_PauseMenuScreenWidget>(GetWorld(), FPCGameInstance->PauseMenuScreenWidgetClass);

	//Get a reference to the Level script actor of the current gameplay level
	SampleGameplayLevelScriptActorRef = Cast<AFPCSampleGameplayLevelScriptActor>(GetWorld()->GetLevelScriptActor());
}
