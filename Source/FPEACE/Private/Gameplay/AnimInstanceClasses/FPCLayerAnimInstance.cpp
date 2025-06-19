// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCLayerAnimInstance.h"
#include "FPCSkeletalAnimInstance.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"

void UFPCLayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningMesh.IsValid() && OwningAnimInstance == nullptr)
		OwningAnimInstance = Cast<UFPCSkeletalAnimInstance>(OwningMesh->GetAnimInstance());
}
