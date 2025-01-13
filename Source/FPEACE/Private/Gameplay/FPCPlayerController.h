// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPCPlayerController.generated.h"

enum class ECameraMode : uint8;
class UFPCGameInstance;
class UFPCAnimInstance;
class UInputAction;
class UInputMappingContext;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraModeChanged, ECameraMode, NewCameraMode);


/**
 * 
 */
UCLASS()
class FPEACE_API AFPCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnCameraModeChanged OnCameraModeChanged;

	void ToggleCameraMode();

	ECameraMode GetCameraMode() const { return CurrentCameraMode; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECameraMode CurrentCameraMode;

	/**
	 * Reference to the Input mapping context object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC")
	TSoftObjectPtr<UInputMappingContext> FPCCharacterInputMapping;

	/*
	 * Reference to the Camera Switching Input Action object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC")
	TSoftObjectPtr<UInputAction> CameraSwitchAction;

	UFUNCTION()
	void SetCharacterCameraMode(ECameraMode newCameraMode);

	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

private:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
};
