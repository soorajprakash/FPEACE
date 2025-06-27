// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCGameplayPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Gameplay/FPCGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
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
	SetShowMouseCursor(bIsGamePaused);
	PauseMenuScreenWidget->TogglePauseMenu(bIsGamePaused);

	// In UE 5.5, at least in the editor, the engine crashes whenever the game is paused while ArmASR is enabled.
	// So, get around it by checking the CVar value of ArmASR and accordingly make sure that it's not active for a few frames after the game is paused
	// There are no issues while unpausing. 
	IConsoleVariable* ArmASRCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ArmASR.Enable"));
	int32 ArmASRCVarValue = -1;
	if (ArmASRCVar)
		ArmASRCVarValue = ArmASRCVar->GetInt();

	if (ArmASRCVarValue > 0)
		ConsoleCommand("r.ArmASR.Enable 0");

	UGameplayStatics::SetGamePaused(GetWorld(), bIsGamePaused);

	if (ArmASRCVarValue > 0)
	{
		FTimerDynamicDelegate ARM_ASR_CrashHandleTimerDelegate = FTimerDynamicDelegate();
		ARM_ASR_CrashHandleTimerDelegate.BindDynamic(this, &AFPCGameplayPlayerController::WorkAroundThirdPartyTemporalUpscalerCrashOnGamePause);
		UKismetSystemLibrary::K2_SetTimerDelegate(ARM_ASR_CrashHandleTimerDelegate, 0.01f, false);
	}
}

void AFPCGameplayPlayerController::WorkAroundThirdPartyTemporalUpscalerCrashOnGamePause()
{
	ConsoleCommand("r.ArmASR.Enable 1");
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
