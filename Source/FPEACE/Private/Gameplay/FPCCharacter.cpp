// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCCharacter.h"

#include "CommonEnums.h"
#include "EngineExtensions/FPCCapsuleComponent.h"
#include "EngineExtensions/FPCCharacterMovementComponent.h"
#include "EngineExtensions/FPCSkeletalMeshComponent.h"
#include "EngineExtensions/FPCCameraComponent.h"
#include "EngineExtensions/FPCSpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "FPCAnimInstance.h"
#include "EngineExtensions/FPCChildActorComponent.h"
#include "EngineExtensions/FPCGameInstance.h"
#include "FPCPlayerController.h"
#include "FPCWeapon.h"
#include "KismetAnimationLibrary.h"
#include "DataStructures/FPCCharacterData.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFPCCharacter::AFPCCharacter(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<UFPCSkeletalMeshComponent>(MeshComponentName).SetDefaultSubobjectClass<UFPCCharacterMovementComponent>(CharacterMovementComponentName).
	                  SetDefaultSubobjectClass<UFPCCapsuleComponent>(CapsuleComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get references to the Extensions 
	TPSBodyMeshComp = Cast<UFPCSkeletalMeshComponent>(GetMesh());
	TPSBodyMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	TPSBodyMeshComp->SetCastHiddenShadow(true);
	FPCCapsuleComp = Cast<UFPCCapsuleComponent>(GetCapsuleComponent());

	// Create Components

	if (!FPSBodyMeshComp)
	{
		FPSBodyMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("FPSBody"));
		FPSBodyMeshComp->SetupAttachment(TPSBodyMeshComp);
		FPSBodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FPSBodyMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
		FPSBodyMeshComp->SetCastShadow(false);
		FPSBodyMeshComp->SetOnlyOwnerSee(true);
	}

	if (!FPCSpringArmComp)
	{
		FPCSpringArmComp = CreateDefaultSubobject<UFPCSpringArmComponent>(TEXT("FPCSpringArm"));
		FPCSpringArmComp->SetupAttachment(FPCCapsuleComp);
		FPCSpringArmComp->bInheritRoll = false;
	}

	if (!FPCCameraComp)
	{
		FPCCameraComp = CreateDefaultSubobject<UFPCCameraComponent>(TEXT("FPCCamera"));
		FPCCameraComp->SetupAttachment(FPCSpringArmComp);
	}
}

// Called when the game starts or when spawned
void AFPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get references to the Extensions 
	FPCMovementComp = Cast<UFPCCharacterMovementComponent>(GetMovementComponent());

	// Set initial Character values
	TargetLocomotionState = ELocomotionState::Running;

	// Store required Character data
	ForwardLimits = GetCharacterData()->CharacterDirectionLimits.ForwardLimits;
	BackwardLimits = GetCharacterData()->CharacterDirectionLimits.BackwardLimits;
	DeadZone = GetCharacterData()->CharacterDirectionLimits.DirectionalDeadzone;
}

void AFPCCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Fetch movement data
	const FVector& CharacterVelocity = FPCMovementComp->Velocity;
	const FVector& CharacterAcceleration = FPCMovementComp->GetCurrentAcceleration();

	CharacterVelocity2D = FVector(CharacterVelocity.X, CharacterVelocity.Y, 0);
	CharacterAcceleration2D = FVector(CharacterAcceleration.X, CharacterAcceleration.Y, 0);

	CharacterAbsoluteSpeed = UKismetMathLibrary::VSizeXY(CharacterVelocity);
	CharacterAbsoluteSpeed2D = UKismetMathLibrary::VSizeXY(CharacterVelocity2D);

	FVector CurrentWorldLocation = GetActorLocation();
	CurrentDeltaDistance = FVector::Distance(LastWorldLocation, CurrentWorldLocation);
	LastWorldLocation = CurrentWorldLocation;

	// Update transition rule values
	UpdateAnimationTransitionRuleValues();

	// Update the locomotion state of the character to reach the target state
	HandleLocomotionStateChange();
}

void AFPCCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TPSMeshAnimInstance = CastChecked<UFPCAnimInstance>(TPSBodyMeshComp->GetAnimInstance());
	TPSMeshAnimInstance->isBaseAnimInstance = true;
	FPSMeshAnimInstance = CastChecked<UFPCAnimInstance>(FPSBodyMeshComp->GetAnimInstance());
	FPCPlayerControllerInstance = CastChecked<AFPCPlayerController>(NewController);

	// Component Settings
	if (FPCPlayerControllerInstance)
	{
		// Bind to Camera Mode switch callback
		FPCPlayerControllerInstance->OnCameraModeChanged.AddDynamic(this, &AFPCCharacter::SetCameraMode);
	}

	// TODO : Remove this after testing
	PickUpAndEquipWeapon(GetCharacterData()->DefaultWeaponBP);
}

// Called to bind functionality to input
void AFPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EInputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EInputComp->BindAction(LookAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &AFPCCharacter::LookAround);
		EInputComp->BindAction(MoveAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &AFPCCharacter::MoveAround);
		EInputComp->BindAction(RunAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCCharacter::ToggleRunSprint);
	}
}

void AFPCCharacter::SetCameraMode(ECameraMode NewCameraMode)
{
	if (FPCPlayerControllerInstance)
	{
		// Set the Anim Transition Value
		IsInTPSCameraMode = NewCameraMode == ECameraMode::TPS;

		// Based on the camera mode, toggle the skeletal meshes
		// The idea is to basically always have the TPS meshes visible to the world (Since it's needed for multiplayer)
		// The FPS meshes is only visible to the owner of the character when in FPS mode and never visible to the world
		TPSBodyMeshComp->SetOwnerNoSee(!IsInTPSCameraMode);
		FPSBodyMeshComp->SetHiddenInGame(IsInTPSCameraMode);
		RefreshWeaponVisibility();


		// Set up the spring Arm and Camera components according to current camera mode
		switch (NewCameraMode)
		{
		case ECameraMode::FPS:
			FPCSpringArmComp->AttachToComponent(FPSBodyMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("SOCKET_Camera"));
			FPCSpringArmComp->TargetArmLength = GetCharacterData()->CameraModeSettings[ECameraMode::FPS].MaxSpringArmLength;
			break;

		case ECameraMode::TPS:

			FPCSpringArmComp->AttachToComponent(FPCCapsuleComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			FPCSpringArmComp->SetArmLengthFromPitch(GetCharacterData()->CameraModeSettings[ECameraMode::TPS].PitchToArmLengthCurve, FPCPlayerControllerInstance->GetControlRotation());
			break;
		}

		// Switch the camera settings defined in the character's data asset
		SetCameraSettings(NewCameraMode);
	}
}

void AFPCCharacter::RefreshWeaponVisibility() const
{
	if (CurrentTPSWeaponRef)
		CurrentTPSWeaponRef->ToggleVisibilityToOwner(IsInTPSCameraMode);

	if (CurrentFPSWeaponRef)
		CurrentFPSWeaponRef->SetActorHiddenInGame(IsInTPSCameraMode);
}

void AFPCCharacter::SetCameraSettings(ECameraMode CurrentCameraMode)
{
	if (GetCharacterData())
	{
		const FCharacterCameraModeSettings& TargetCameraModeSettings = FPCCharacterData->CameraModeSettings[CurrentCameraMode];

		FPCSpringArmComp->SetRelativeLocation(TargetCameraModeSettings.SpringArmTramsformOffset);
		FPCSpringArmComp->SocketOffset = TargetCameraModeSettings.SpringArmSocketOffset;
		FPCSpringArmComp->TargetOffset = TargetCameraModeSettings.SpringArmTargetOffset;
		FPCSpringArmComp->bUsePawnControlRotation = TargetCameraModeSettings.bUsePawnControlRotation;
		FPCSpringArmComp->bEnableCameraLag = TargetCameraModeSettings.bEnableCameraLag;
		FPCSpringArmComp->CameraLagSpeed = TargetCameraModeSettings.CameraLagSpeed;
		FPCSpringArmComp->CameraLagMaxDistance = TargetCameraModeSettings.CameraLagMaxDistance;
	}
}

void AFPCCharacter::HandleLocomotionStateChange()
{
	// Don't change the locomotion state if the character is pivoting
	if (!IsCharacterPivoting)
	{
		// Set the character to default locomotion target state if it's not moving
		// TODO: Allow the default state to be set somewhere globally
		if (!IsCharacterMoving && TargetLocomotionState != ELocomotionState::Running)
			TargetLocomotionState = ELocomotionState::Running;

		//Check if the character is in a state where it can only walk.
		//For Example, when in Crouch state or ADS state or is armed but moving sideways or backwards etc
		bool canOnlyWalk = bIsCharacterCrouched || bIsCharacterInProneState || bIsCharacterInADSState || (bIsCharacterArmed && CurrentVelocityDirection != ELocomotionDirection::Forward);

		if (canOnlyWalk)
			SetCurrentLocomotionStateWithSettings(ELocomotionState::Walking);
		else if (currentLocomotionState != TargetLocomotionState)
		{
			bool canOnlyRun = bIsCharacterArmed && CurrentVelocityDirection != ELocomotionDirection::Forward && currentLocomotionState != ELocomotionState::Running;
			SetCurrentLocomotionStateWithSettings(canOnlyRun ? ELocomotionState::Running : TargetLocomotionState);
		}
	}
}

void AFPCCharacter::SetCurrentLocomotionState(ELocomotionState newLocomotionState)
{
	if (currentLocomotionState != newLocomotionState)
	{
		currentLocomotionState = newLocomotionState;
		OnLocomotionStateChanged.Broadcast(currentLocomotionState);
	}
}

void AFPCCharacter::SetCurrentLocomotionStateWithSettings(ELocomotionState newLocomotionState)
{
	if (currentLocomotionState != newLocomotionState)
	{
		SetCurrentLocomotionState(newLocomotionState);
		SetLocomotionStateSettings(currentLocomotionState);
	}
}

void AFPCCharacter::LinkCombatAnimClassToCharacter(FName AnimClassNameToLink) const
{
	if (TPSMeshAnimInstance->CurrentLinkedAnimInstance)
	{
		// TPSBodyMeshComp->LinkAnimClassLayers(nullptr);
		// TPSBodyMeshComp->SetAnimInstanceClass(nullptr);
		// TPSBodyMeshComp->SetAnimInstanceClass(TPSMeshAnimInstance->GetClass());
		// TPSBodyMeshComp->AnimScriptInstance = TPSMeshAnimInstance;
		// if (const USkeletalMeshComponent* SkeletalMeshComp = TPSBodyMeshComp)
		// {
		// 	const TArray<UAnimInstance*>& LinkedAnimInstances = SkeletalMeshComp->GetLinkedAnimInstances();
		// 	TArray<UFPCAnimInstance*> LinkedFPCAnimInstances;
		// 	for (int i = 0; i < LinkedAnimInstances.Num(); i++)
		// 		LinkedFPCAnimInstances.Add(Cast<UFPCAnimInstance>(LinkedAnimInstances[i]));
		//
		// 	for (UFPCAnimInstance* LinkedFPCAnimInstance : LinkedFPCAnimInstances)
		// 	{
		// 		LinkedFPCAnimInstance->UninitializeAnimation();
		// 		LinkedFPCAnimInstance->MarkAsGarbage();
		// 		UE_LOG(LogTemp, Warning, TEXT("Unlinking Anim Class %s : %s"), *LinkedFPCAnimInstance->GetClass()->GetName(),
		// 		       LinkedFPCAnimInstance->bCreatedByLinkedAnimGraph ? TEXT("True") : TEXT("False"));
		// 		TPSBodyMeshComp->UnlinkAnimClassLayers(LinkedFPCAnimInstance->GetClass());
		// 	}
		// }

		TPSBodyMeshComp->UnlinkAnimClassLayers(TPSMeshAnimInstance->CurrentLinkedAnimInstance->GetClass());
		TPSMeshAnimInstance->CurrentLinkedAnimInstance = nullptr;
	}

	UClass* TPSCombatAnimClass = TPSMeshAnimInstance->GetAnimClassFor(ECameraMode::TPS, AnimClassNameToLink, FString(TEXT("Linking Anim Class To Character"))).LoadSynchronous();
	TPSBodyMeshComp->LinkAnimClassLayers(TPSCombatAnimClass);
	TPSMeshAnimInstance->CurrentLinkedAnimInstance = Cast<UFPCAnimInstance>(TPSBodyMeshComp->GetLinkedAnimLayerInstanceByClass(TPSCombatAnimClass));

	if (FPSMeshAnimInstance->CurrentLinkedAnimInstance)
	{
		FPSBodyMeshComp->UnlinkAnimClassLayers(FPSMeshAnimInstance->CurrentLinkedAnimInstance->GetClass());
		FPSMeshAnimInstance->CurrentLinkedAnimInstance = nullptr;

		FPSMeshAnimInstance->CurrentLinkedAnimInstance = nullptr;
	}

	UClass* FPSCombatAnimClass = FPSMeshAnimInstance->GetAnimClassFor(ECameraMode::FPS, AnimClassNameToLink, FString(TEXT("Linking Anim Class To Character"))).LoadSynchronous();
	FPSBodyMeshComp->LinkAnimClassLayers(FPSCombatAnimClass);
	FPSMeshAnimInstance->CurrentLinkedAnimInstance = Cast<UFPCAnimInstance>(FPSBodyMeshComp->GetLinkedAnimLayerInstanceByClass(FPSCombatAnimClass));
}

void AFPCCharacter::PickUpAndEquipWeapon(const TSubclassOf<AFPCWeapon>& WeaponBP)
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

	if (WeaponBP.Get() == nullptr)
		LinkCombatAnimClassToCharacter(TEXT("Unarmed"));
	else
	{
		// Spawn one weapon instance for each FPS and TPS mesh
		CurrentFPSWeaponRef = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBP));
		CurrentTPSWeaponRef = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBP));

		if (CurrentFPSWeaponRef && CurrentTPSWeaponRef)
		{
			CurrentFPSWeaponRef->SetupWeapon(ECameraMode::FPS, FPSBodyMeshComp);
			CurrentTPSWeaponRef->SetupWeapon(ECameraMode::TPS, TPSBodyMeshComp);

			// Can use the name from either of the weapon refs since they are the same
			LinkCombatAnimClassToCharacter(CurrentFPSWeaponRef->WeaponAnimLayerClassName);
		}
	}

	bIsCharacterArmed = WeaponBP != nullptr;
	RefreshWeaponVisibility();
}

ELocomotionDirection AFPCCharacter::CalculateLocomotionDirection(const float LocomotionDirectionAngle, const ELocomotionDirection CurrentDirection) const
{
	// First check for deadzones

	//If current direction is forward and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Forward && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.X - DeadZone, ForwardLimits.Y + DeadZone))
		return ELocomotionDirection::Forward;
	//If current direction is backward and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Backward && (LocomotionDirectionAngle < BackwardLimits.X + DeadZone || LocomotionDirectionAngle > BackwardLimits.Y - DeadZone))
		return ELocomotionDirection::Backward;
	//If current direction is right and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Right && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.Y - DeadZone, BackwardLimits.Y + DeadZone))
		return ELocomotionDirection::Right;
	//If current direction is left and direction angle is within deadzone buffer, keep the direction
	if (CurrentDirection == ELocomotionDirection::Left && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, BackwardLimits.X - DeadZone, ForwardLimits.X + DeadZone))
		return ELocomotionDirection::Left;

	// If the above conditions weren't met, calculate the direction manually

	//Check for Forward range
	if (UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.X, ForwardLimits.Y))
		return ELocomotionDirection::Forward;

	// Check for Backward range
	if (LocomotionDirectionAngle < BackwardLimits.X || LocomotionDirectionAngle > BackwardLimits.Y)
		return ELocomotionDirection::Backward;

	//Check for right and left. We only need to test if it's positive angle since we've already ruled out forward and backward
	return LocomotionDirectionAngle > 0 ? ELocomotionDirection::Right : ELocomotionDirection::Left;
}

void AFPCCharacter::SetCurrentVelocityDirection(ELocomotionDirection newVelocityDirection)
{
	if (CurrentVelocityDirection == newVelocityDirection)
		return;

	CurrentVelocityDirection = newVelocityDirection;
}

void AFPCCharacter::SetCurrentAccelerationDirection(ELocomotionDirection newAccelerationDirection)
{
	if (CurrentVelocityDirection == newAccelerationDirection)
		return;

	CurrentAccelerationDirection = newAccelerationDirection;
}

void AFPCCharacter::AddControllerPitchInput(float Val)
{
	if (FPCPlayerControllerInstance && GetCharacterData())
	{
		FRotator ControlRotation = FPCPlayerControllerInstance->GetControlRotation();
		ControlRotation.Pitch -= Val; // Subtracting instead of adding since we are normailizing the values in the next step.
		ControlRotation.Pitch = FRotator::NormalizeAxis(ControlRotation.Pitch); // Normalizing flips the direction.

		// Clamp the pitch between the limits defined in character data asset
		ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch, -FPCCharacterData->ControllerRotationPitchClamp, FPCCharacterData->ControllerRotationPitchClamp);

		// Set the updated control rotation
		FPCPlayerControllerInstance->SetControlRotation(ControlRotation);
	}
}

void AFPCCharacter::AddControllerYawInput(float Val)
{
	if (FPCPlayerControllerInstance && GetCharacterData())
	{
		FRotator ControlRotation = FPCPlayerControllerInstance->GetControlRotation();
		ControlRotation.Yaw += Val;
		ControlRotation.Pitch = FRotator::NormalizeAxis(ControlRotation.Pitch); // Normalizing flips the direction.

		// Set the updated control rotation
		FPCPlayerControllerInstance->SetControlRotation(ControlRotation);
	}
}

void AFPCCharacter::LookAround(const FInputActionValue& InputActionValue)
{
	FVector2D input = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(input.X);
	AddControllerPitchInput(input.Y);

	CharacterLookSpineVertical.Roll = -Controller->GetControlRotation().Pitch;

	if (FPCPlayerControllerInstance->GetCameraMode() == ECameraMode::TPS)
		FPCSpringArmComp->SetArmLengthFromPitch(GetCharacterData()->CameraModeSettings[ECameraMode::TPS].PitchToArmLengthCurve, FPCPlayerControllerInstance->GetControlRotation());
}

void AFPCCharacter::MoveAround(const FInputActionValue& InputActionValue)
{
	FVector2D Input = InputActionValue.Get<FVector2D>();
	Input = Input.GetRotated(-GetControlRotation().Yaw); // Rotate the input to face the character's direction
	FVector Input3D = FVector(Input.Y, Input.X, 0);
	AddMovementInput(Input3D);

	//Calculate the direction angles
	AccelerationDirectionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterAcceleration2D, GetActorRotation());
	VelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterVelocity2D, GetActorRotation());

	// Calculate Play rate
	CurrentAnimPlayRate = FMath::Clamp(CharacterAbsoluteSpeed2D / CurrentMaxLocomotionSpeed, 0.f, 1.f);

	SetCurrentAccelerationDirection(CalculateLocomotionDirection(AccelerationDirectionAngle, CurrentAccelerationDirection));
	SetCurrentVelocityDirection(CalculateLocomotionDirection(VelocityDirectionAngle, CurrentVelocityDirection));
}

void AFPCCharacter::ToggleRunSprint()
{
	TargetLocomotionState = TargetLocomotionState != ELocomotionState::Running ? ELocomotionState::Running : ELocomotionState::Sprinting;
}

void AFPCCharacter::SetLocomotionStateSettings(ELocomotionState newLocomotionState)
{
	CurrentMaxLocomotionSpeed = GetCharacterData()->LocomotionStateSettings[newLocomotionState].MaxWalkSpeed;
	CurrentLocomotionStateFloat = UKismetMathLibrary::Conv_ByteToDouble(static_cast<uint8>(newLocomotionState));

	const FLocomotionStateSetting& StateSettings = FPCCharacterData->LocomotionStateSettings[newLocomotionState];
	FPCMovementComp->bUseSeparateBrakingFriction = StateSettings.bUseSeparateBrakingFriction;
	FPCMovementComp->MaxWalkSpeed = StateSettings.MaxWalkSpeed;
	FPCMovementComp->MaxAcceleration = StateSettings.MaxAcceleration;
	FPCMovementComp->BrakingDecelerationWalking = StateSettings.BrakingDeceleration;
	FPCMovementComp->BrakingFrictionFactor = StateSettings.BrakingFrictionFactor;
	FPCMovementComp->BrakingFriction = StateSettings.BrakingFriction;
}

void AFPCCharacter::UpdateAnimationTransitionRuleValues()
{
	// Update if the character is moving
	if (IsCharacterPivoting)
		IsCharacterMoving = true;
	else
	{
		if (IsCharacterMoving && CharacterVelocity2D.Length() < 1)
			IsCharacterMoving = false;
		else if (!IsCharacterMoving && CharacterVelocity2D.Length() > 10)
			IsCharacterMoving = true;
	}

	// Update if the character is accelerating
	IsCharacterAccelerating = !CharacterAcceleration2D.IsNearlyZero();

	// Update if character has reached close to max speed
	HasCharacterReachedCurrentMaxSpeed = CharacterAbsoluteSpeed2D > CurrentMaxLocomotionSpeed * 0.8f;

	//	--------------------- CHECK FOR PIVOTING ---------------------

	// Check for character pivoting
	TriggerCharacterRePivot = false;

	// Precompute normalized velocity and acceleration for clarity
	const FVector NormalizedVelocity = CharacterVelocity2D.GetSafeNormal();
	const FVector NormalizedAcceleration = CharacterAcceleration2D.GetSafeNormal();

	// Calculate the dot product once
	const float VelocityAccelerationDot = FVector::DotProduct(NormalizedVelocity, NormalizedAcceleration);
	const float AccelerationDeltaDot = FVector::DotProduct(PrevNormalizedAcceleration, NormalizedAcceleration);

	if (IsCharacterPivoting)
	{
		// Check if the character is in the stopping part of the pivot or the starting part
		IsCharacterInPivotStoppingState = VelocityAccelerationDot < 0;

		// Check if the character has stopped pivoting or needs to trigger a re-pivot
		if (FMath::IsNearlyEqual(VelocityAccelerationDot, 1.0f, 0.01f))
			IsCharacterPivoting = false;
		else if (AccelerationDeltaDot < 0)
			TriggerCharacterRePivot = true;
	}
	// Start pivoting if the dot product indicates a reversal
	else if (AccelerationDeltaDot < 0 && !FMath::IsNearlyZero(AccelerationDeltaDot))
		IsCharacterPivoting = true;

	// Store the current values for comparison in the next frame
	PrevVelocityAccelerationDot = VelocityAccelerationDot;
	PrevNormalizedAcceleration = NormalizedAcceleration;
}

UFPCCharacterData* AFPCCharacter::GetCharacterData()
{
	// Get the Character Data asset reference
	if (FPCCharacterData == nullptr)
		FPCCharacterData = UFPCGameInstance::GetInstance(this)->CharacterData;

	return FPCCharacterData;
}
