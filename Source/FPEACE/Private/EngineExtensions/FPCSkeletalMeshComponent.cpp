// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCSkeletalMeshComponent.h"

UFPCSkeletalMeshComponent::UFPCSkeletalMeshComponent() : Super()
{
	VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
}
