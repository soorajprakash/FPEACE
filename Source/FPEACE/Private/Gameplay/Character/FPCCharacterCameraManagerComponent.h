// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "Gameplay/FPCActorComponent.h"
#include "FPCCharacterCameraManagerComponent.generated.h"


class UFPCSkeletalMeshComponent;
class UFPCCharacterData;
class AFPCPlayerController;
class UFPCSpringArmComponent;
class UFPCCameraComponent;
class AFPCCharacter;
enum class ECameraMode : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCameraModeChanged,ECameraMode,NewCameraMode);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPEACE_API UFPCCharacterCameraManagerComponent : public UFPCActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPCCharacterCameraManagerComponent();

	UPROPERTY(BlueprintReadOnly)
	bool IsInTPSCameraMode = false;

	void UpdateCameraState() const;

	void ToggleCameraMode();

	FCameraModeChanged OnCameraModeChanged;
	
protected:

	UPROPERTY(BlueprintReadOnly)
	ECameraMode CurrentCameraMode = ECameraMode::TPS;
	
	//	--------------------- OVERRIDES ---------------------

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	TObjectPtr<AFPCCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UFPCCameraComponent> FPCCameraComp;

	UPROPERTY()
	TObjectPtr<UFPCSpringArmComponent> FPCSpringArmComp;

	UPROPERTY()
	TObjectPtr<AFPCPlayerController> PlayerControllerRef;

	UPROPERTY()
	TObjectPtr<UFPCCharacterData> FPCCharacterData;

	UPROPERTY()
	TObjectPtr<UFPCSkeletalMeshComponent> TPSBodyMeshComp;

	UPROPERTY()
	TObjectPtr<UFPCSkeletalMeshComponent> FPSBodyMeshComp;
	
	/*
	 * Sets the Character to either use FPS camera mode or TPS camera mode
	 */
	UFUNCTION()
	void SetCameraMode(ECameraMode NewCameraMode);

	/*
	 * Changes the settings for the camera and the spring arm for the given mode using the character data
	 */
	void SetCameraSettings(ECameraMode NewCameraMode) const;
};