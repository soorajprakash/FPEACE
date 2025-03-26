// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCBlueprintFunctionLibrary.h"

bool UFPCBlueprintFunctionLibrary::GetIfAnimSequenceHasRootMotionDelta(UAnimSequence* TargetSequence)
{
	const FVector RootMotionTranslation = TargetSequence->ExtractRootMotionFromRange(0.0f, TargetSequence->GetPlayLength()).GetTranslation();
	const float RootMotionDistance = RootMotionTranslation.Size2D();
	return !FMath::IsNearlyZero(RootMotionDistance);
}

bool UFPCBlueprintFunctionLibrary::GetIfAnimSequenceHasLoopingEnabled(UAnimSequence* TargetSequence)
{
	if (!TargetSequence)
		return false;
	return TargetSequence->bLoop;
}

EAdditiveAnimationType UFPCBlueprintFunctionLibrary::GetAnimSequenceAdditiveType(UAnimSequence* TargetSequence)
{
	return TargetSequence->GetAdditiveAnimType();
}
