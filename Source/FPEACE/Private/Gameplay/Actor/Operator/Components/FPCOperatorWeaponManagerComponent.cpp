// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCOperatorWeaponManagerComponent.h"
#include "FPCOperatorCameraManagerComponent.h"
#include "FPCOperatorMovementComponent.h"
#include "Gameplay/Weapon/FPCWeapon.h"
#include "FCTween.h"
#include "AnimNotifies/FPCCharacterAnimationStateChangedNotify.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/Weapon/FPCGun.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UFPCOperatorWeaponManagerComponent::UFPCOperatorWeaponManagerComponent(): CurrentWeaponLocationLag(), CurrentWeaponRotationLag(),
                                                                            CurrentWeaponRotationLagVector(),
                                                                            CurrentADSBlendFactor(0),
                                                                            CurrentWeaponHandIKLocationOffset(),
                                                                            CurrentWeaponHandIKRotationOffset(), ADSStateChanged(false), RecentlyUsedWeaponStateChanged(false),
                                                                            currentWeaponSatchelIndex(0),
                                                                            ADSBlendFactorTween(nullptr), bLastFrameWantsADSState(false), bLastFrameWeaponRecentlyUsedState(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UFPCOperatorWeaponManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (FPCOperatorCameraManagerComp.IsValid())
		FPCOperatorCameraManagerComp->OnCameraModeChanged.AddDynamic(this, &UFPCOperatorWeaponManagerComponent::CharacterCameraModeChanged);

	if (FPCOperatorMovementComp.IsValid())
		FPCOperatorMovementComp->OnCurrentLocomotionStateChanged.AddDynamic(this, &UFPCOperatorWeaponManagerComponent::CharacterCurrentLocomotionStateChanged);

	// Register to the ADS Anim Notify callback
	UFPCCharacterAnimationStateChangedNotify::OnAnimationStateChanged.AddDynamic(this, &UFPCOperatorWeaponManagerComponent::OnADSAnimStateChanged);
}

void UFPCOperatorWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Generate the weapon satchel
	GenerateSatchel();

	// Equip the first weapon in satchel
	if (WeaponSatchel.Num() > 0)
		EquipWeapon(WeaponSatchel[0]);
}

void UFPCOperatorWeaponManagerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateStateChanges();
	CalculateCurrentWeaponLagValues();
}

void UFPCOperatorWeaponManagerComponent::UpdateWeaponVisibility(const bool IsInTPSCameraMode) const
{
	if (CurrentTPSWeaponRef)
		CurrentTPSWeaponRef->ToggleVisibilityToOwner(IsInTPSCameraMode);

	if (CurrentFPSWeaponRef)
		CurrentFPSWeaponRef->SetActorHiddenInGame(IsInTPSCameraMode);
}

void UFPCOperatorWeaponManagerComponent::SwitchADSState(bool UseADS)
{
	bWantsToADS = UseADS && bIsCharacterArmed;

	ToggleADSBlendFactor(bWantsToADS);
	FPCOperatorCameraManagerComp->SwitchCameraFOV(UseADS);
	SetCurrentWeaponHandIKOffset();
}

void UFPCOperatorWeaponManagerComponent::ToggleADSBlendFactor(const int targetBlendFactor)
{
	if (ADSBlendFactorTween)
		ADSBlendFactorTween->Destroy();
	
	ADSBlendFactorTween = FCTween::Play(CurrentADSBlendFactor, targetBlendFactor, [&](float V) { CurrentADSBlendFactor = V; }, CurrentWeaponAnimSettings.FocusTime);
}

void UFPCOperatorWeaponManagerComponent::ToggleWeaponUse(const bool UseWeapon)
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

		GetWorld()->GetTimerManager().SetTimer(WeaponUseCoolDownTimer, WeaponUseCoolDownDelegate, FPCOperatorData->WeaponUseCoolDownTime, false);
	}
}

void UFPCOperatorWeaponManagerComponent::TryCurrentGunReload() const
{
	if (CurrentFPSGunRef && CurrentTPSGunRef)
	{
		CurrentFPSGunRef->TryBeginReload();
		CurrentTPSGunRef->TryBeginReload();
	}
}

void UFPCOperatorWeaponManagerComponent::CycleWeaponInSatchel(bool bGoNext)
{
	currentWeaponSatchelIndex += bGoNext ? 1 : -1;

	if (currentWeaponSatchelIndex > WeaponSatchel.Num() - 1)
		currentWeaponSatchelIndex = 0;
	else if (currentWeaponSatchelIndex < 0)
		currentWeaponSatchelIndex = WeaponSatchel.Num() - 1;

	EquipWeapon(WeaponSatchel[currentWeaponSatchelIndex]);
}

void UFPCOperatorWeaponManagerComponent::OnADSAnimStateChanged(ENotifyAnimationType AnimType, ENotifyAnimationEventType AnimEventType)
{
	if (AnimType == EnterADS || AnimType == ExitADS)
	{
		bIsADSInProgress = AnimEventType == Started;
		bIsCharacterInADSState = bIsADSInProgress != bWantsToADS;
	}
}

void UFPCOperatorWeaponManagerComponent::OnGunReloadStart(bool bEmptyReload, AFPCGun* ReloadingGun)
{
	ToggleADSBlendFactor(0);
}

void UFPCOperatorWeaponManagerComponent::OnGunReloadComplete(AFPCGun* ReloadingGun)
{
	ToggleADSBlendFactor(1);
}

void UFPCOperatorWeaponManagerComponent::EquipWeapon(const FWeaponSatchelItem& SatchelItem)
{
	if (CurrentFPSWeaponRef && CurrentTPSWeaponRef)
	{
		//Toggle the current weapons off
		CurrentFPSWeaponRef->ToggleActor(false);
		CurrentTPSWeaponRef->ToggleActor(false);

		// Unbind callbacks from current guns (if the weapon is a gun)
		if (CurrentFPSGunRef && CurrentTPSGunRef)
		{
			CurrentFPSGunRef->OnReloadStarted.RemoveAll(this);
			CurrentTPSGunRef->OnReloadStarted.RemoveAll(this);

			CurrentFPSGunRef->OnReloadFinished.RemoveAll(this);
			CurrentTPSGunRef->OnReloadFinished.RemoveAll(this);

			CurrentFPSGunRef->OnMagWasEmptied.RemoveAll(this);
			CurrentTPSGunRef->OnMagWasEmptied.RemoveAll(this);
		}
	}

	if (SatchelItem.FPSWeaponIns && SatchelItem.TPSWeaponIns)
	{
		// Spawn one weapon instance for each FPS and TPS mesh
		CurrentFPSWeaponRef = SatchelItem.FPSWeaponIns;
		CurrentFPSGunRef = Cast<AFPCGun>(CurrentFPSWeaponRef);
		CurrentTPSWeaponRef = SatchelItem.TPSWeaponIns;
		CurrentTPSGunRef = Cast<AFPCGun>(CurrentTPSWeaponRef);

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

	if (CurrentFPSWeaponRef && CurrentTPSWeaponRef)
	{
		//Toggle the new weapons on
		CurrentFPSWeaponRef->ToggleActor(true);
		CurrentTPSWeaponRef->ToggleActor(true);

		// Bind callbacks to current guns (if the weapon is a gun)
		if (CurrentFPSGunRef && CurrentTPSGunRef)
		{
			CurrentFPSGunRef->OnReloadStarted.AddDynamic(this, &UFPCOperatorWeaponManagerComponent::OnGunReloadStart);
			CurrentTPSGunRef->OnReloadStarted.AddDynamic(this, &UFPCOperatorWeaponManagerComponent::OnGunReloadStart);

			CurrentFPSGunRef->OnReloadFinished.AddDynamic(this, &UFPCOperatorWeaponManagerComponent::OnGunReloadComplete);
			CurrentTPSGunRef->OnReloadFinished.AddDynamic(this, &UFPCOperatorWeaponManagerComponent::OnGunReloadComplete);
		}
	}

	bIsCharacterArmed = CurrentFPSWeaponRef && CurrentTPSWeaponRef;

	SwitchADSState(bWantsToADS);

	UpdateWeaponVisibility(FPCOperatorCameraManagerComp->IsInTPSCameraMode);

	SetCurrentWeaponHandIKOffset();

	// Can use the name from either FPS or TPS weapon ref since they will be the same
	OnNewWeaponEquipped.Broadcast(CurrentFPSWeaponRef, CurrentTPSWeaponRef);
}

void UFPCOperatorWeaponManagerComponent::GenerateSatchel()
{
	if (FPCOperatorData.IsValid())
		for (TSubclassOf WeaponClass : FPCOperatorData->InitialWeaponsInSatchel)
			AddNewWeaponToSatchel(WeaponClass);
}

void UFPCOperatorWeaponManagerComponent::AddNewWeaponToSatchel(const TSubclassOf<AFPCWeapon> WeaponBP)
{
	FWeaponSatchelItem NewSatchelItem = FWeaponSatchelItem();
	if (WeaponBP != nullptr)
	{
		NewSatchelItem.FPSWeaponIns = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBP));
		NewSatchelItem.FPSWeaponIns->SetupWeapon(ECameraMode::FPS, FPSBodyMeshComp.Get());
		NewSatchelItem.FPSWeaponIns->ToggleActor(false);


		NewSatchelItem.TPSWeaponIns = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBP));
		NewSatchelItem.TPSWeaponIns->SetupWeapon(ECameraMode::TPS, MainBodyMeshComp.Get());
		NewSatchelItem.TPSWeaponIns->ToggleActor(false);
	}

	WeaponSatchel.Add(NewSatchelItem);
}

void UFPCOperatorWeaponManagerComponent::CharacterCameraModeChanged(ECameraMode NewCameraMode)
{
	UpdateWeaponVisibility(NewCameraMode == ECameraMode::TPS);
}

void UFPCOperatorWeaponManagerComponent::CharacterCurrentLocomotionStateChanged(ELocomotionState)
{
	SetCurrentWeaponHandIKOffset();
}

void UFPCOperatorWeaponManagerComponent::SetCurrentWeaponHandIKOffset()
{
	if (!CurrentFPSWeaponRef || !CurrentTPSWeaponRef)
		return;

	FVector StartLocationValue = CurrentWeaponHandIKLocationOffset;
	FQuat StartRotationValue = CurrentWeaponHandIKRotationOffset.Quaternion();

	if (bWantsToADS)
	{
		FCTween::Play(StartLocationValue, CurrentWeaponAnimSettings.ADSLocomotionStateOffsets[FPCOperatorMovementComp->GetCurrentLocomotionState()].GetLocation(),
		              [&](FVector V) { CurrentWeaponHandIKLocationOffset = V; }, 0.25f, EFCEase::Smoothstep);

		FCTween::Play(StartRotationValue, CurrentWeaponAnimSettings.ADSLocomotionStateOffsets[FPCOperatorMovementComp->GetCurrentLocomotionState()].Rotator().Quaternion(),
		              [&](FQuat Q) { CurrentWeaponHandIKRotationOffset = Q.Rotator(); }, 0.25f, EFCEase::Smoothstep);
	}
	else
	{
		FCTween::Play(StartLocationValue, CurrentWeaponAnimSettings.DefaultLocomotionStateOffsets[FPCOperatorMovementComp->GetCurrentLocomotionState()].GetLocation(),
		              [&](FVector V) { CurrentWeaponHandIKLocationOffset = V; }, 0.25f, EFCEase::Smoothstep);

		FCTween::Play(StartRotationValue, CurrentWeaponAnimSettings.DefaultLocomotionStateOffsets[FPCOperatorMovementComp->GetCurrentLocomotionState()].Rotator().Quaternion(),
		              [&](FQuat Q) { CurrentWeaponHandIKRotationOffset = Q.Rotator(); }, 0.25f, EFCEase::Smoothstep);
	}
}

void UFPCOperatorWeaponManagerComponent::UpdateStateChanges()
{
	ADSStateChanged = bWantsToADS != bLastFrameWantsADSState;
	bLastFrameWantsADSState = bWantsToADS;

	RecentlyUsedWeaponStateChanged = bWasWeaponUsedRecently != bLastFrameWeaponRecentlyUsedState;
	bLastFrameWeaponRecentlyUsedState = bWasWeaponUsedRecently;
}

void UFPCOperatorWeaponManagerComponent::CalculateCurrentWeaponLagValues()
{
	float ClampedPitchDelta = FMath::Clamp(FPCOperatorCameraManagerComp->GetCameraPitchDelta(), -1, 1);
	float ClampedYawDelta = FMath::Clamp(FPCOperatorMovementComp->GetCharacterYawDelta(), -1, 1);
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
