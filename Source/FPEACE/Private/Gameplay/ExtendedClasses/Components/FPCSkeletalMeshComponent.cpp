// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCSkeletalMeshComponent.h"

UFPCSkeletalMeshComponent::UFPCSkeletalMeshComponent() : Super()
{
	VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
}

void UFPCSkeletalMeshComponent::FPC_SetOwnerNoSee(bool bNoSee)
{
	SetOwnerNoSee(bNoSee);
	OnOwnerNoSeeStateChanged.Broadcast(bNoSee);
}

void UFPCSkeletalMeshComponent::FPC_SetOnlyOwnerSee(bool bSee)
{
	SetOnlyOwnerSee(bSee);
	OnOnlyOwnerSeeStateChanged.Broadcast(bSee);
}
