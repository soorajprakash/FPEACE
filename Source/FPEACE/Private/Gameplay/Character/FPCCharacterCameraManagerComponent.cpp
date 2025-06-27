// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCCharacterCameraManagerComponent.h"
#include "CommonEnums.h"
#include "CommonStructs.h"
#include "FCTween.h"
#include "FPCCameraComponent.h"
#include "FPCCharacter.h"
#include "FPCCharacterWeaponManagerComponent.h"
#include "FPCGameplayPlayerController.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/FPCSpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UFPCCharacterCameraManagerComponent::SetCameraMode(ECameraMode NewCameraMode)
{
	CurrentCameraMode = NewCameraMode;

	// Set the Anim Transition Value
	IsInTPSCameraMode = CurrentCameraMode == ECameraMode::TPS;

	// Based on the camera mode, toggle the skeletal meshes
	// The idea is to basically always have the TPS meshes visible to the world (Since it's needed for multiplayer)
	// The FPS meshes is only visible to the owner of the character when in FPS mode and never visible to the world
	TPSBodyMeshComp->FPC_SetOwnerNoSee(!IsInTPSCameraMode);
	FPSArmsMeshComp->SetHiddenInGame(IsInTPSCameraMode);
	FPSLowerBodyMeshComp->SetHiddenInGame(IsInTPSCameraMode);

	// Set up the spring Arm and Camera components according to current camera mode
	switch (CurrentCameraMode)
	{
	case ECameraMode::FPS:
		FPCSpringArmComp->AttachToComponent(FPSArmsMeshComp.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("SOCKET_Camera"));
		FPCSpringArmComp->TargetArmLength = FPCCharacterData->CameraModeSettings[ECameraMode::FPS].MaxSpringArmLength;
		break;

	case ECameraMode::TPS:

		FPCSpringArmComp->AttachToComponent(TPSBodyMeshComp.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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

float UFPCCharacterCameraManagerComponent::GetTargetFOV(const ECameraMode TargetCameraMode) const
{
	float currentDefaultFOV = TargetCameraMode == ECameraMode::TPS ? DefaultTPSCameraFieldOfView : DefaultFPSCameraFieldOfView;
	float ADSFOVMultiplier = TargetCameraMode == ECameraMode::TPS
		                         ? FPCCharacterData->CameraModeSettings[ECameraMode::TPS].DefaultAimFOVMultiplier
		                         : FPCWeaponManagerComp->GetCurrentWeaponAnimSettings().FirstPersonADSFieldOfViewMultiplier;
	return FPCWeaponManagerComp->GetWantsToAds() ? currentDefaultFOV * ADSFOVMultiplier : currentDefaultFOV;
}

void UFPCCharacterCameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (OwningCharacter == nullptr)
		OwningCharacter = Cast<AFPCCharacter>(GetOwner());

	if (OwningCharacter.IsValid())
	{
		FPCWeaponManagerComp = OwningCharacter->GetFPCCharacterWeaponManager();
		FPCSpringArmComp = OwningCharacter->GetFPCSpringArmComp();
		FPCCameraComp = OwningCharacter->GetCharacterCameraComp();
		FPCCharacterData = OwningCharacter->GetCharacterData();
		TPSBodyMeshComp = OwningCharacter->GetTPSBodyMeshComp();
		FPSArmsMeshComp = OwningCharacter->GetFPSArmsMeshComp();
		FPSLowerBodyMeshComp = OwningCharacter->GetFPSLowerBodyMeshComp();
	}

	// Setup Values
	if (FPCCharacterData.IsValid())
	{
		DefaultTPSCameraFieldOfView = FPCCharacterData->CameraModeSettings[ECameraMode::TPS].DefaultCameraFOV;
		DefaultFPSCameraFieldOfView = FPCCharacterData->CameraModeSettings[ECameraMode::FPS].DefaultCameraFOV;
	}
}

void UFPCCharacterCameraManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwningCharacter.IsValid())
	{
		PlayerControllerRef = OwningCharacter->GetFPCPlayerController();
	}

	// Set Initial camera mode
	SetCameraMode(FPCCharacterData->StartingCameraMode);
}

void UFPCCharacterCameraManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
UFPCCharacterCameraManagerComponent::UFPCCharacterCameraManagerComponent(): CameraPitchDelta(0), PrevCameraRotation(), CameraFOVTween(nullptr), DefaultTPSCameraFieldOfView(0),
                                                                            DefaultFPSCameraFieldOfView(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
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

void UFPCCharacterCameraManagerComponent::SwitchCameraFOV(bool UseADS)
{
	if (CameraFOVTween)
		CameraFOVTween->Destroy();

	float startingFOV = FPCCameraComp->FieldOfView;
	CameraFOVTween = FCTween::Play(startingFOV, GetTargetFOV(CurrentCameraMode), [&](float Value) { FPCCameraComp->SetFieldOfView(Value); },
	                               FPCWeaponManagerComp->GetCurrentWeaponAnimSettings().FocusTime);
}
