// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCOperatorCameraManagerComponent.h"
#include "FCTween.h"
#include "FPCOperatorWeaponManagerComponent.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/Actor/FPCGameplayPlayerController.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/ExtendedClasses/Components/FPCCameraComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UFPCOperatorCameraManagerComponent::SetCameraMode(ECameraMode NewCameraMode)
{
	CurrentCameraMode = NewCameraMode;

	// Set the Anim Transition Value
	IsInTPSCameraMode = CurrentCameraMode == ECameraMode::TPS;

	// Based on the camera mode, toggle the skeletal meshes
	// The idea is to basically always have the TPS meshes visible to the world (Since it's needed for multiplayer)
	// The FPS meshes is only visible to the owner of the character when in FPS mode and never visible to the world
	MainBodyMeshComp->FPC_SetOwnerNoSee(!IsInTPSCameraMode);
	FPSBodyMeshComp->SetHiddenInGame(IsInTPSCameraMode);
	FPSLowerBodyMeshComp->SetHiddenInGame(IsInTPSCameraMode);

	// Set up the spring Arm and Camera components according to current camera mode
	switch (CurrentCameraMode)
	{
	case ECameraMode::FPS:
		FPCSpringArmComp->AttachToComponent(FPSBodyMeshComp.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("SOCKET_Camera"));
		FPCSpringArmComp->TargetArmLength = FPCOperatorData->CameraModeSettings[ECameraMode::FPS].MaxSpringArmLength;
		break;

	case ECameraMode::TPS:

		FPCSpringArmComp->AttachToComponent(MainBodyMeshComp.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FPCSpringArmComp->SetArmLengthFromPitch(FPCOperatorData->CameraModeSettings[ECameraMode::TPS].PitchToArmLengthCurve, OwningOperator->GetFPCPlayerController()->GetControlRotation());
		break;
	}

	// Switch the camera settings defined in the character's data asset
	SetCameraSettings(CurrentCameraMode);

	OnCameraModeChanged.Broadcast(CurrentCameraMode);
}

void UFPCOperatorCameraManagerComponent::SetCameraSettings(ECameraMode NewCameraMode) const
{
	const FCharacterCameraModeSettings& TargetCameraModeSettings = FPCOperatorData->CameraModeSettings[NewCameraMode];

	// Spring Arm Settings
	FPCSpringArmComp->SetRelativeLocation(TargetCameraModeSettings.SpringArmTramsformOffset);
	FPCSpringArmComp->SocketOffset = TargetCameraModeSettings.SpringArmSocketOffset;
	FPCSpringArmComp->TargetOffset = TargetCameraModeSettings.SpringArmTargetOffset;
	FPCSpringArmComp->bUsePawnControlRotation = TargetCameraModeSettings.bUsePawnControlRotation;
	FPCSpringArmComp->bEnableCameraLag = TargetCameraModeSettings.bEnableCameraLag;
	FPCSpringArmComp->CameraLagSpeed = TargetCameraModeSettings.CameraLagSpeed;
	FPCSpringArmComp->CameraLagMaxDistance = TargetCameraModeSettings.CameraLagMaxDistance;

	// Camera Settings
	FPCCameraComp->SetFieldOfView(GetTargetFOV(NewCameraMode));
}

float UFPCOperatorCameraManagerComponent::GetTargetFOV(const ECameraMode TargetCameraMode) const
{
	float currentDefaultFOV = TargetCameraMode == ECameraMode::TPS ? DefaultTPSCameraFieldOfView : DefaultFPSCameraFieldOfView;
	float ADSFOVMultiplier = TargetCameraMode == ECameraMode::TPS
		                         ? FPCOperatorData->CameraModeSettings[ECameraMode::TPS].DefaultAimFOVMultiplier
		                         : FPCOperatorWeaponManagerComp->GetCurrentWeaponAnimSettings().FirstPersonADSFieldOfViewMultiplier;
	return FPCOperatorWeaponManagerComp->GetWantsToAds() ? currentDefaultFOV * ADSFOVMultiplier : currentDefaultFOV;
}

void UFPCOperatorCameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (OwningOperator.IsValid())
	{
		FPCCameraComp = OwningOperator->GetCharacterCameraComp();
	}

	// Setup Values
	if (FPCOperatorData.IsValid())
	{
		DefaultTPSCameraFieldOfView = FPCOperatorData->CameraModeSettings[ECameraMode::TPS].DefaultCameraFOV;
		DefaultFPSCameraFieldOfView = FPCOperatorData->CameraModeSettings[ECameraMode::FPS].DefaultCameraFOV;
	}
}

void UFPCOperatorCameraManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set Initial camera mode
	SetCameraMode(FPCOperatorData->StartingCameraMode);
}

void UFPCOperatorCameraManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator CurrentCameraRotation = FPCCameraComp->GetComponentRotation();
	CameraPitchDelta = CurrentCameraRotation.Pitch - PrevCameraRotation.Pitch;
	PrevCameraRotation = CurrentCameraRotation;

	FHitResult HitResult;
	FVector CameraLocation = FPCCameraComp->GetComponentLocation();
	FVector EndLocation = CameraLocation + FPCCameraComp->GetForwardVector() * 10000;
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), CameraLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>{}, EDrawDebugTrace::None, HitResult,
	                                          true))
		CurrentCameraLookAtHit = HitResult.Location;
	else
		CurrentCameraLookAtHit = EndLocation;
}

// Sets default values for this component's properties
UFPCOperatorCameraManagerComponent::UFPCOperatorCameraManagerComponent(): CameraPitchDelta(0), PrevCameraRotation(), CameraFOVTween(nullptr), DefaultTPSCameraFieldOfView(0),
                                                                            DefaultFPSCameraFieldOfView(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UFPCOperatorCameraManagerComponent::UpdateCameraState() const
{
	if (IsInTPSCameraMode)
		FPCSpringArmComp->SetArmLengthFromPitch(FPCOperatorData->CameraModeSettings[ECameraMode::TPS].PitchToArmLengthCurve, PlayerControllerRef->GetControlRotation());
}

void UFPCOperatorCameraManagerComponent::ToggleCameraMode()
{
	SetCameraMode(CurrentCameraMode == ECameraMode::FPS ? ECameraMode::TPS : ECameraMode::FPS);
}

void UFPCOperatorCameraManagerComponent::SwitchCameraFOV(bool UseADS)
{
	if (CameraFOVTween)
		CameraFOVTween->Destroy();

	float startingFOV = FPCCameraComp->FieldOfView;
	CameraFOVTween = FCTween::Play(startingFOV, GetTargetFOV(CurrentCameraMode), [&](float Value) { FPCCameraComp->SetFieldOfView(Value); },
	                               FPCOperatorWeaponManagerComp->GetCurrentWeaponAnimSettings().FocusTime);
}
