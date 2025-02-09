// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCCharacterCameraManagerComponent.h"
#include "CommonEnums.h"
#include "CommonStructs.h"
#include "FPCCharacter.h"
#include "FPCPlayerController.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/FPCSpringArmComponent.h"


void UFPCCharacterCameraManagerComponent::SetCameraMode(ECameraMode NewCameraMode)
{
	CurrentCameraMode = NewCameraMode;

	// Set the Anim Transition Value
	IsInTPSCameraMode = CurrentCameraMode == ECameraMode::TPS;

	// Based on the camera mode, toggle the skeletal meshes
	// The idea is to basically always have the TPS meshes visible to the world (Since it's needed for multiplayer)
	// The FPS meshes is only visible to the owner of the character when in FPS mode and never visible to the world
	TPSBodyMeshComp->SetOwnerNoSee(!IsInTPSCameraMode);
	FPSBodyMeshComp->SetHiddenInGame(IsInTPSCameraMode);

	// Set up the spring Arm and Camera components according to current camera mode
	switch (CurrentCameraMode)
	{
	case ECameraMode::FPS:
		FPCSpringArmComp->AttachToComponent(FPSBodyMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("SOCKET_Camera"));
		FPCSpringArmComp->TargetArmLength = FPCCharacterData->CameraModeSettings[ECameraMode::FPS].MaxSpringArmLength;
		break;

	case ECameraMode::TPS:

		FPCSpringArmComp->AttachToComponent(TPSBodyMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FPCSpringArmComp->SetArmLengthFromPitch(FPCCharacterData->CameraModeSettings[ECameraMode::TPS].PitchToArmLengthCurve, OwningCharacter->GetFPCPlayerController()->GetControlRotation());
		break;
	}

	// Switch the camera settings defined in the character's data asset
	SetCameraSettings(CurrentCameraMode);

	OnCameraModeChanged.Broadcast(CurrentCameraMode);
}

void UFPCCharacterCameraManagerComponent::SetCameraSettings(ECameraMode NewCameraMode) const
{
	const FCharacterCameraModeSettings& TargetCameraModeSettings = FPCCharacterData->CameraModeSettings[NewCameraMode];

	FPCSpringArmComp->SetRelativeLocation(TargetCameraModeSettings.SpringArmTramsformOffset);
	FPCSpringArmComp->SocketOffset = TargetCameraModeSettings.SpringArmSocketOffset;
	FPCSpringArmComp->TargetOffset = TargetCameraModeSettings.SpringArmTargetOffset;
	FPCSpringArmComp->bUsePawnControlRotation = TargetCameraModeSettings.bUsePawnControlRotation;
	FPCSpringArmComp->bEnableCameraLag = TargetCameraModeSettings.bEnableCameraLag;
	FPCSpringArmComp->CameraLagSpeed = TargetCameraModeSettings.CameraLagSpeed;
	FPCSpringArmComp->CameraLagMaxDistance = TargetCameraModeSettings.CameraLagMaxDistance;
}

void UFPCCharacterCameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (OwningCharacter == nullptr)
		OwningCharacter = Cast<AFPCCharacter>(GetOwner());

	if (OwningCharacter)
	{
		FPCSpringArmComp = OwningCharacter->GetFPCSpringArmComp();
		FPCCameraComp = OwningCharacter->GetFPCCameraComp();
		FPCCharacterData = OwningCharacter->GetCharacterData();
		TPSBodyMeshComp = OwningCharacter->GetTPSBodyMeshComp();
		FPSBodyMeshComp = OwningCharacter->GetFPSBodyMeshComp();
	}
}

void UFPCCharacterCameraManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwningCharacter)
	{
		PlayerControllerRef = OwningCharacter->GetFPCPlayerController();
	}

	// Set Initial camera mode
	SetCameraMode(FPCCharacterData->StartingCameraMode);
}

// Sets default values for this component's properties
UFPCCharacterCameraManagerComponent::UFPCCharacterCameraManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UFPCCharacterCameraManagerComponent::UpdateCameraState() const
{
	if (IsInTPSCameraMode)
		FPCSpringArmComp->SetArmLengthFromPitch(FPCCharacterData->CameraModeSettings[ECameraMode::TPS].PitchToArmLengthCurve, PlayerControllerRef->GetControlRotation());
}

void UFPCCharacterCameraManagerComponent::ToggleCameraMode()
{
	SetCameraMode(CurrentCameraMode == ECameraMode::FPS ? ECameraMode::TPS : ECameraMode::FPS);
}
