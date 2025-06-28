// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCOperatorUtilityAnimInstance.h"
#include "FPCOperatorSkeletalAnimInstance.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"

void UFPCOperatorUtilityAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningMesh.IsValid() && OwningAnimInstance == nullptr)
		OwningAnimInstance = Cast<UFPCOperatorSkeletalAnimInstance>(OwningMesh->GetAnimInstance());
}
