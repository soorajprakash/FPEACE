// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPCGameplayPlayerController.generated.h"

class AFPCOperator;
enum class ECommonInputType : uint8;
class UFPEACE_HUD_Widget;
class UFPCFullScreenJoystickComponent;
class UFPCGameInstance;
class UFPEACE_CUI_PauseMenuScreenWidget;
class AFPCSampleGameplayLevelScriptActor;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class FPEACE_API AFPCGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/*
	 * Holds the state of whether the game is currently paused
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bIsGamePaused = false;

	/**
	 * Reference to the Input mapping context object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TSoftObjectPtr<UInputMappingContext> FPCCharacterInputMapping;

	/**
	 * Reference to the Input mapping context object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TSoftObjectPtr<UInputMappingContext> FPCGlobalInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="FPEACE|Input")
	TSoftObjectPtr<UInputAction> PauseMenuToggleAction;

	UPROPERTY()
	TObjectPtr<UFPCGameInstance> FPCGameInstance;

	// Reference to the Pause Menu Screen Widget that was created in the level
	UPROPERTY()
	TObjectPtr<UFPEACE_CUI_PauseMenuScreenWidget> PauseMenuScreenWidget;

	/*
	 * Reference to the HUD class used for touch input
	 */
	UPROPERTY(EditDefaultsOnly, Category="FPEACE")
	TSubclassOf<UUserWidget> TouchHUDClass;

	/*
	 * Reference to the HUD class used for KBM and Controller input
	 */
	UPROPERTY(EditDefaultsOnly, Category="FPEACE")
	TSubclassOf<UUserWidget> DesktopHUDClass;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	UFUNCTION(BlueprintCallable)
	void RemoveAllHUD();
	
private:

	/*
	 * Reference to the operator controlled by this player controller
	 */
	TWeakObjectPtr<AFPCOperator> ControlledPlayerOperator;
	
	// HUD Widget instance References
	UPROPERTY()
	TWeakObjectPtr<UFPEACE_HUD_Widget> TouchHUD = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UFPEACE_HUD_Widget> DesktopHUD = nullptr;

	UPROPERTY()
	TObjectPtr<AFPCSampleGameplayLevelScriptActor> SampleGameplayLevelScriptActorRef;

	/*
	 * Reference to the custom virtual Joystick component on the Operator
	 */
	UPROPERTY()
	TWeakObjectPtr<UFPCFullScreenJoystickComponent> VirtualJoystickComp = nullptr;

	void HandleInputMethodChanged(ECommonInputType NewType);
	void HandleHUDAndInputForInputDevice(ECommonInputType Type);
	
	UFUNCTION()
	void WorkAroundArmASRCrashOnGamePause();

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
};
