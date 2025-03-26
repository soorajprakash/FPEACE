// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "AnimNotifies/FPCCharacterAnimationStateChangedNotify.h"

FAnimationStateChangeNotify UFPCCharacterAnimationStateChangedNotify::OnAnimationStateChanged;

void UFPCCharacterAnimationStateChangedNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	OnAnimationStateChanged.Broadcast(AnimationType, AnimationEventType);
}
