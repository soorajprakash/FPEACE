// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCCharacterWeaponManagerComponent.h"
#include "CommonEnums.h"
#include "FPCCharacter.h"
#include "FPCCharacterCameraManagerComponent.h"
#include "FPCCharacterMovementComponent.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/Weapon/FPCWeapon.h"
#include "FCTween.h"
#include "FPCCharacterAnimationManagerComponent.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/AnimInstanceClasses/FPCLayerAnimInstance.h"
#include "Gameplay/AnimInstanceClasses/FPCSkeletalAnimInstance.h"
#include "Gameplay/Weapon/RecoilHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UFPCCharacterWeaponManagerComponent::UFPCCharacterWeaponManagerComponent(): CurrentWeaponLocationLag(), CurrentWeaponRotationLag(),
                                                                            CurrentWeaponRotationLagVector(),
                                                                            CurrentADSBlendFactor(0),
                                                                            CurrentWeaponHandIKLocationOffset(),
                                                                            CurrentWeaponHandIKRotationOffset(), ADSStateChanged(false),
                                                                            ADSBlendFactorTween(nullptr), bLastFrameWantsADSState(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UFPCCharacterWeaponManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (OwningCharacter == nullptr)
		OwningCharacter = Cast<AFPCCharacter>(GetOwner());

	if (OwningCharacter)
	{
		OwningCharacterMovementComp = OwningCharacter->GetCharacterMovementComponent();
		FPCCharacterData = OwningCharacter->GetCharacterData();
		TPSBodyMeshComp = OwningCharacter->GetTPSBodyMeshComp();
		FPSBodyMeshComp = OwningCharacter->GetFPSArmsMeshComp();
		FPCCameraManagerComp = OwningCharacter->GetFPCCharacterCameraManager();
		FPCAnimationManagerComp = OwningCharacter->GetFPCCharacterAnimationManager();
	}

	if (FPCCameraManagerComp)
		FPCCameraManagerComp->OnCameraModeChanged.AddDynamic(this, &UFPCCharacterWeaponManagerComponent::CharacterCameraModeChanged);

	if (OwningCharacterMovementComp)
		OwningCharacterMovementComp->OnCurrentLocomotionStateChanged.AddDynamic(this, &UFPCCharacterWeaponManagerComponent::CharacterCurrentLocomotionStateChanged);

	// Register to the ADS Anim Notify callback
	UFPCCharacterAnimationStateChangedNotify::OnAnimationStateChanged.AddDynamic(this, &UFPCCharacterWeaponManagerComponent::OnADSAnimStateChanged);
}

void UFPCCharacterWeaponManagerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateStateChanges();
	CalculateCurrentWeaponLagValues();
}

void UFPCCharacterWeaponManagerComponent::UpdateWeaponVisibility(const bool IsInTPSCameraMode) const
{
	if (CurrentTPSWeaponRef)
		CurrentTPSWeaponRef->ToggleVisibilityToOwner(IsInTPSCameraMode);

	if (CurrentFPSWeaponRef)
		CurrentFPSWeaponRef->SetActorHiddenInGame(IsInTPSCameraMode);
}

void UFPCCharacterWeaponManagerComponent::SwitchADSState(bool UseADS)
{
	bWantsToADS = UseADS && bIsCharacterArmed;

	if (ADSBlendFactorTween)
		ADSBlendFactorTween->Destroy();

	ADSBlendFactorTween = FCTween::Play(CurrentADSBlendFactor, bWantsToADS, [&](float V) { CurrentADSBlendFactor = V; }, CurrentWeaponAnimSettings.FocusTime);

	SetCurrentWeaponHandIKOffset();
}

void UFPCCharacterWeaponManagerComponent::ToggleWeaponUse(const bool UseWeapon)
{
	CurrentFPSWeaponRef->SetIsWeaponInUse(UseWeapon);
	CurrentTPSWeaponRef->SetIsWeaponInUse(UseWeapon);

	bWantsToUseWeapon = UseWeapon;

	if (UseWeapon)
	{
		bWasWeaponUsedRecently = true;
		WeaponUseCoolDownTimer.Invalidate();
	}
	else
	{
		FTimerDelegate WeaponUseCoolDownDelegate = FTimerDelegate::CreateLambda([this]
		{
			bWasWeaponUsedRecently = false;
		});

		GetWorld()->GetTimerManager().SetTimer(WeaponUseCoolDownTimer, WeaponUseCoolDownDelegate, FPCCharacterData->WeaponUseCoolDownTime, false);
	}
}

void UFPCCharacterWeaponManagerComponent::TryWeaponReload() const
{
	if (CurrentFPSGunRef && CurrentTPSGunRef)
	{
		CurrentFPSGunRef->TryBeginReload();
		CurrentTPSGunRef->TryBeginReload();
	}
}

void UFPCCharacterWeaponManagerComponent::OnADSAnimStateChanged(ENotifyAnimationType AnimType, ENotifyAnimationEventType AnimEventType)
{
	if (AnimType == EnterADS || AnimType == ExitADS)
	{
		bIsADSInProgress = AnimEventType == Started;
		bIsCharacterInADSState = bIsADSInProgress != bWantsToADS;
	}
}

void UFPCCharacterWeaponManagerComponent::EquipWeapon(const TSoftClassPtr<AFPCWeapon>& WeaponBP)
{
	// Destroy current weapon instance and disarm the character
	if (CurrentFPSWeaponRef)
	{
		CurrentFPSWeaponRef->Destroy();
		CurrentFPSWeaponRef = nullptr;
	}

	if (CurrentTPSWeaponRef)
	{
		CurrentTPSWeaponRef->Destroy();
		CurrentTPSWeaponRef = nullptr;
	}

	if (WeaponBP != nullptr)
	{
		UClass* WeaponBPRef = WeaponBP.LoadSynchronous();
		// Spawn one weapon instance for each FPS and TPS mesh
		CurrentFPSWeaponRef = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBPRef));
		CurrentFPSGunRef = Cast<AFPCGun>(CurrentFPSWeaponRef);
		CurrentTPSWeaponRef = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBPRef));
		CurrentTPSGunRef = Cast<AFPCGun>(CurrentTPSWeaponRef);

		if (CurrentFPSWeaponRef && CurrentTPSWeaponRef)
		{
			CurrentFPSWeaponRef->SetupWeapon(ECameraMode::FPS, FPSBodyMeshComp);
			CurrentTPSWeaponRef->SetupWeapon(ECameraMode::TPS, TPSBodyMeshComp);
		}

		CurrentWeaponAnimSettings = CurrentFPSWeaponRef->GetAnimSettings();
		CurrentWeaponLagSettings = CurrentFPSWeaponRef->GetLagSettings();
		CurrentWeaponAimSocketOffset = CurrentFPSWeaponRef->GetAimSocketTransform();
		CurrentWeaponEmitterSocketOffset = CurrentFPSWeaponRef->GetEmitterSocketTransform();
	}
	else
	{
		CurrentWeaponAnimSettings = FWeaponAnimSettings();
		CurrentWeaponLagSettings = FWeaponLagSettings();
		CurrentWeaponAimSocketOffset = FTransform::Identity;
		CurrentWeaponEmitterSocketOffset = FTransform::Identity;
	}

	bIsCharacterArmed = WeaponBP != nullptr;

	SwitchADSState(bWantsToADS);

	UpdateWeaponVisibility(FPCCameraManagerComp->IsInTPSCameraMode);

	SetCurrentWeaponHandIKOffset();

	// Can use the name from either FPS or TPS weapon ref since they will be the same
	OnNewWeaponEquipped.Broadcast(CurrentFPSWeaponRef, CurrentTPSWeaponRef);
}

void UFPCCharacterWeaponManagerComponent::CharacterCameraModeChanged(ECameraMode NewCameraMode)
{
	UpdateWeaponVisibility(NewCameraMode == ECameraMode::TPS);
}

void UFPCCharacterWeaponManagerComponent::CharacterCurrentLocomotionStateChanged(ELocomotionState)
{
	SetCurrentWeaponHandIKOffset();
}

void UFPCCharacterWeaponManagerComponent::SetCurrentWeaponHandIKOffset()
{
	if (!CurrentFPSWeaponRef || !CurrentTPSWeaponRef)
		return;

	FVector StartLocationValue = CurrentWeaponHandIKLocationOffset;
	FQuat StartRotationValue = CurrentWeaponHandIKRotationOffset.Quaternion();

	if (bWantsToADS)
	{
		FCTween::Play(StartLocationValue, CurrentWeaponAnimSettings.ADSLocomotionStateOffsets[OwningCharacterMovementComp->GetCurrentLocomotionState()].GetLocation(),
		              [&](FVector V) { CurrentWeaponHandIKLocationOffset = V; }, 0.25f, EFCEase::Smoothstep);

		FCTween::Play(StartRotationValue, CurrentWeaponAnimSettings.ADSLocomotionStateOffsets[OwningCharacterMovementComp->GetCurrentLocomotionState()].Rotator().Quaternion(),
		              [&](FQuat Q) { CurrentWeaponHandIKRotationOffset = Q.Rotator(); }, 0.25f, EFCEase::Smoothstep);
	}
	else
	{
		FCTween::Play(StartLocationValue, CurrentWeaponAnimSettings.DefaultLocomotionStateOffsets[OwningCharacterMovementComp->GetCurrentLocomotionState()].GetLocation(),
		              [&](FVector V) { CurrentWeaponHandIKLocationOffset = V; }, 0.25f, EFCEase::Smoothstep);

		FCTween::Play(StartRotationValue, CurrentWeaponAnimSettings.DefaultLocomotionStateOffsets[OwningCharacterMovementComp->GetCurrentLocomotionState()].Rotator().Quaternion(),
		              [&](FQuat Q) { CurrentWeaponHandIKRotationOffset = Q.Rotator(); }, 0.25f, EFCEase::Smoothstep);
	}
}

void UFPCCharacterWeaponManagerComponent::UpdateStateChanges()
{
	ADSStateChanged = bWantsToADS != bLastFrameWantsADSState;
	bLastFrameWantsADSState = bWantsToADS;
}

void UFPCCharacterWeaponManagerComponent::CalculateCurrentWeaponLagValues()
{
	float ClampedPitchDelta = FMath::Clamp(FPCCameraManagerComp->GetCameraPitchDelta(), -1, 1);
	float ClampedYawDelta = FMath::Clamp(OwningCharacterMovementComp->GetCharacterYawDelta(), -1, 1);
	FWeaponLagSettingSection CurrentLagSection = bWantsToADS ? CurrentWeaponLagSettings.ADSWeaponLag : CurrentWeaponLagSettings.DefaultWeaponLag;

	FVector TargetWeaponLocationLag = ClampedPitchDelta * CurrentLagSection.VerticalLocationLag + ClampedYawDelta * CurrentLagSection.HorizontalLocationLag;
	FVector TargetWeaponRotationLag = ClampedPitchDelta * CurrentLagSection.VerticalRotationLag + ClampedYawDelta * CurrentLagSection.HorizontalRotationLag;


	CurrentWeaponLocationLag = UKismetMathLibrary::VectorSpringInterp(CurrentWeaponLocationLag, TargetWeaponLocationLag, LocationLagSpringState, CurrentLagSection.Stiffness, CurrentLagSection.Damping,
	                                                                  UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), CurrentLagSection.Mass);

	CurrentWeaponRotationLagVector = UKismetMathLibrary::VectorSpringInterp(CurrentWeaponRotationLagVector, TargetWeaponRotationLag, RotationLagSpringState, CurrentLagSection.Stiffness,
	                                                                        CurrentLagSection.Damping,
	                                                                        UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), CurrentLagSection.Mass);
	CurrentWeaponRotationLag = FRotator(CurrentWeaponRotationLagVector.Y, CurrentWeaponRotationLagVector.Z, CurrentWeaponRotationLagVector.X);
}
