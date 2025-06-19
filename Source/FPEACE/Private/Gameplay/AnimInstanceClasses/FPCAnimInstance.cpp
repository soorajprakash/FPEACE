// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCAnimInstance.h"

#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/Character/FPCCharacter.h"


void UFPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	InitializeReferences();
}

void UFPCAnimInstance::RenameAnimInstanceObject(const FString& NewName)
{
#if WITH_EDITOR
	FName UniqueName = MakeUniqueObjectName(GetOuter(), GetClass(), FName(*NewName), EUniqueObjectNameOptions::None);
	Rename(*UniqueName.ToString());
#endif
}

void UFPCAnimInstance::InitializeReferences()
{
	if (OwningCharacter == nullptr)
		OwningCharacter = Cast<AFPCCharacter>(TryGetPawnOwner());

	if (OwningMesh == nullptr)
		OwningMesh = Cast<UFPCSkeletalMeshComponent>(GetOwningComponent());

	if (OwningCharacter.IsValid())
	{
		if (!OwningCharacterMovementComponent.IsValid())
			OwningCharacterMovementComponent = OwningCharacter->GetCharacterMovementComponent();

		if (!OwningCharacterCameraManager.IsValid())
			OwningCharacterCameraManager = OwningCharacter->GetFPCCharacterCameraManager();

		if (!OwningCharacterWeaponManager.IsValid())
			OwningCharacterWeaponManager = OwningCharacter->GetFPCCharacterWeaponManager();

		if (!OwningCharacterAnimationManager.IsValid())
			OwningCharacterAnimationManager = OwningCharacter->GetFPCCharacterAnimationManager();
	}
}
