// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPCGameplayPlayerController.generated.h"

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

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

private:
	UPROPERTY()
	TObjectPtr<AFPCSampleGameplayLevelScriptActor> SampleGameplayLevelScriptActorRef;

	UFUNCTION()
	void WorkAroundThirdPartyTemporalUpscalerCrashOnGamePause();
	
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
};
