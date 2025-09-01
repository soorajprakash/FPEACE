// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCBlueprintFunctionLibrary.h"
#include "SequenceEvaluatorLibrary.h"
#include "AnimNodes/AnimNode_SequenceEvaluator.h"
#include "Gameplay/Common/CommonEnums.h"
#include "Gameplay/Common/CommonStructs.h"
#include "Kismet/KismetSystemLibrary.h"

FString UFPCBlueprintFunctionLibrary::GetAnimSequenceName(const UAnimSequence* TargetSequence)
{
	return UKismetSystemLibrary::GetDisplayName(TargetSequence);
}

bool UFPCBlueprintFunctionLibrary::GetIfAnimSequenceHasRootMotionDelta(UAnimSequence* TargetSequence)
{
	FAnimExtractContext NewContext;
	const FVector RootMotionTranslation = TargetSequence->ExtractRootMotionFromRange(0.0f, TargetSequence->GetPlayLength(), NewContext).GetTranslation();
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

void UFPCBlueprintFunctionLibrary::SetEvaluatorNormalizedTime(const FSequenceEvaluatorReference& SequenceEvaluatorNode, const float NormalizedTime)
{
	if (GetIfEvaluatorHasValidSequence(SequenceEvaluatorNode))
	{
		FAnimNode_SequenceEvaluator& EvaluatorNode = SequenceEvaluatorNode.GetAnimNode<FAnimNode_SequenceEvaluator>();
		EvaluatorNode.SetExplicitTime(EvaluatorNode.GetSequence()->GetPlayLength() * NormalizedTime);
	}
}

EAdditiveAnimationType UFPCBlueprintFunctionLibrary::GetAnimSequenceAdditiveType(UAnimSequence* TargetSequence)
{
	if (!TargetSequence)
		return AAT_None;

	return TargetSequence->GetAdditiveAnimType();
}

bool UFPCBlueprintFunctionLibrary::DoAnimSetsMatchDirectionally(const FDirectionalAnimSet& Rhs, const FDirectionalAnimSet& Lhs)
{
	return Rhs.Forward == Lhs.Forward && Rhs.Backward == Lhs.Backward && Rhs.Left == Lhs.Left && Rhs.Right == Lhs.Right;
}

UAnimSequence* UFPCBlueprintFunctionLibrary::GetDirectionalAnim(const FDirectionalAnimSet& AnimSet, const ELocomotionDirection& Direction)
{
	UAnimSequence* ReturnSequence = nullptr;

	switch (Direction)
	{
	case ELocomotionDirection::Center:
		ReturnSequence = AnimSet.Forward;
		break;
	case ELocomotionDirection::Forward:
		ReturnSequence = AnimSet.Forward;
		break;
	case ELocomotionDirection::Backward:
		ReturnSequence = AnimSet.Backward;
		break;
	case ELocomotionDirection::Left:
		ReturnSequence = AnimSet.Left;
		break;
	case ELocomotionDirection::Right:
		ReturnSequence = AnimSet.Right;
		break;
	}

	return ReturnSequence;
}

bool UFPCBlueprintFunctionLibrary::GripMatchesDirectional(const FDirectionalAnimSet& AnimSet)
{
	return AnimSet.WeaponHandGripPose == AnimSet.Forward || AnimSet.WeaponHandGripPose == AnimSet.Backward || AnimSet.WeaponHandGripPose == AnimSet.Left || AnimSet.WeaponHandGripPose == AnimSet.Right;
}
