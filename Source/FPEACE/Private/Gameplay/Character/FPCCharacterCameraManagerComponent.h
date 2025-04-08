// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "Gameplay/FPCActorComponent.h"
#include "FPCCharacterCameraManagerComponent.generated.h"


class FCTweenInstanceFloat;
class UFPCCharacterWeaponManagerComponent;
class UFPCSkeletalMeshComponent;
class UFPCCharacterData;
class AFPCPlayerController;
class UFPCSpringArmComponent;
class UFPCCameraComponent;
class AFPCCharacter;
enum class ECameraMode : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCameraModeChanged, ECameraMode, NewCameraMode);

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

	void SwitchCameraFOV(bool UseADS);

	FCameraModeChanged OnCameraModeChanged;

	//	--------------------- GETTERS ---------------------

	ECameraMode GetCurrentCameraMode() const { return CurrentCameraMode; }

	FVector GetCurrentCameraLookAtHit() const { return CurrentCameraLookAtHit; }

	float GetCameraPitchDelta() const { return CameraPitchDelta; }

protected:
	UPROPERTY(BlueprintReadOnly)
	ECameraMode CurrentCameraMode = ECameraMode::TPS;

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentCameraLookAtHit = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	float CameraPitchDelta;

	//	--------------------- OVERRIDES ---------------------

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<AFPCCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UFPCCameraComponent> FPCCameraComp;

	UPROPERTY()
	TObjectPtr<UFPCCharacterWeaponManagerComponent> FPCWeaponManagerComp;

	UPROPERTY()
	TObjectPtr<UFPCSpringArmComponent> FPCSpringArmComp;

	UPROPERTY()
	TObjectPtr<AFPCPlayerController> PlayerControllerRef;

	UPROPERTY()
	TObjectPtr<UFPCCharacterData> FPCCharacterData;

	UPROPERTY()
	TObjectPtr<UFPCSkeletalMeshComponent> TPSBodyMeshComp;

	UPROPERTY()
	TObjectPtr<UFPCSkeletalMeshComponent> FPSArmsMeshComp;

	UPROPERTY()
	TObjectPtr<UFPCSkeletalMeshComponent> FPSLowerBodyMeshComp;

	FRotator PrevCameraRotation;

	/*
	 * Instance of the tweener that will be used to tween the camera FOV
	 */
	FCTweenInstanceFloat* CameraFOVTween;

	/*
	 * The default field of view for third person mode when the character is not aiming
	 */
	float DefaultTPSCameraFieldOfView;

	/*
	 * The default field of view for first person mode when the character is not aiming
	 */
	float DefaultFPSCameraFieldOfView;

	/*
	 * Sets the Character to either use FPS camera mode or TPS camera mode
	 */
	UFUNCTION()
	void SetCameraMode(ECameraMode NewCameraMode);

	/*
	 * Changes the settings for the camera and the spring arm for the given mode using the character data
	 */
	void SetCameraSettings(ECameraMode NewCameraMode) const;

	float GetTargetFOV(const ECameraMode TargetCameraMode) const;
};
