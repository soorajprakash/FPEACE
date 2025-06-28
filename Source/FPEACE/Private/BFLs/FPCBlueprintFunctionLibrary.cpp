// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCBlueprintFunctionLibrary.h"

#include "SequenceEvaluatorLibrary.h"
#include "AnimNodes/AnimNode_SequenceEvaluator.h"

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

bool UFPCBlueprintFunctionLibrary::GetIfEvaluatorHasValidSequence(const FSequenceEvaluatorReference& SequenceEvaluatorNode)
{
	return SequenceEvaluatorNode.GetAnimNode<FAnimNode_SequenceEvaluator>().GetSequence() != nullptr;
}

bool UFPCBlueprintFunctionLibrary::GetIfEvaluatorSequenceIsValidOrEqual(const FSequenceEvaluatorReference& SequenceEvaluatorNode, const UAnimSequenceBase* Sequence)
{
	UAnimSequenceBase* EvaluatorSequence = SequenceEvaluatorNode.GetAnimNode<FAnimNode_SequenceEvaluator>().GetSequence();

	if (EvaluatorSequence == nullptr)
		return false;
	if (EvaluatorSequence == Sequence)
		return true;

	return false;
}

EAdditiveAnimationType UFPCBlueprintFunctionLibrary::GetAnimSequenceAdditiveType(UAnimSequence* TargetSequence)
{
	if (!TargetSequence)
		return AAT_None;

	return TargetSequence->GetAdditiveAnimType();
}
