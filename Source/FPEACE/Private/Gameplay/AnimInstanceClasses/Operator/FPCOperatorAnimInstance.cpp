// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCOperatorAnimInstance.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"


void UFPCOperatorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	InitializeReferences();
}

void UFPCOperatorAnimInstance::RenameAnimInstanceObject(const FString& NewName)
{
#if WITH_EDITOR
	FName UniqueName = MakeUniqueObjectName(GetOuter(), GetClass(), FName(*NewName), EUniqueObjectNameOptions::None);
	Rename(*UniqueName.ToString());
#endif
}

void UFPCOperatorAnimInstance::InitializeReferences()
{
	if (OwningOperator == nullptr)
		OwningOperator = Cast<AFPCOperator>(TryGetPawnOwner());

	if (OwningMesh == nullptr)
		OwningMesh = Cast<UFPCSkeletalMeshComponent>(GetOwningComponent());

	if (OwningOperator.IsValid())
	{
		if (!OwningCharacterMovementComponent.IsValid())
			OwningCharacterMovementComponent = OwningOperator->GetCharacterMovementComponent();

		if (!OwningCharacterCameraManager.IsValid())
			OwningCharacterCameraManager = OwningOperator->GetFPCCharacterCameraManager();

		if (!OwningCharacterWeaponManager.IsValid())
			OwningCharacterWeaponManager = OwningOperator->GetFPCCharacterWeaponManager();

		if (!OwningCharacterAnimationManager.IsValid())
			OwningCharacterAnimationManager = OwningOperator->GetFPCCharacterAnimationManager();
	}
}
