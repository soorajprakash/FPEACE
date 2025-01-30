// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FAnimNode_SetRelativeTransformFromPose.h"
#include "Animation/AnimInstanceProxy.h"

void FAnimNode_SetRelativeTransformFromPose::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	Super::Initialize_AnyThread(Context);
	ReferencePose.Initialize(Context);
}

void FAnimNode_SetRelativeTransformFromPose::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	Super::CacheBones_AnyThread(Context);
	ReferencePose.CacheBones(Context);
	for (FBonePair& Pair : BonePairs)
	{
		Pair.TargetBone.Initialize(Context.AnimInstanceProxy->GetRequiredBones());
		Pair.RelativeBone.Initialize(Context.AnimInstanceProxy->GetRequiredBones());
	}
}

bool FAnimNode_SetRelativeTransformFromPose::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	// Validate that all bone pairs are valid
	for (const FBonePair& Pair : BonePairs)
	{
		if (!Pair.TargetBone.IsValidToEvaluate(RequiredBones) || !Pair.RelativeBone.IsValidToEvaluate(RequiredBones))
		{
			return false;
		}
	}
	return BonePairs.Num() > 0;
}

void FAnimNode_SetRelativeTransformFromPose::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	for (FBonePair& Pair : BonePairs)
	{
		Pair.TargetBone.Initialize(RequiredBones);
		Pair.RelativeBone.Initialize(RequiredBones);
	}
}

void FAnimNode_SetRelativeTransformFromPose::UpdateInternal(const FAnimationUpdateContext& Context)
{
	FAnimNode_SkeletalControlBase::UpdateInternal(Context);

	ReferencePose.Update(Context);
}

void FAnimNode_SetRelativeTransformFromPose::EvaluateSkeletalControl_AnyThread(
	FComponentSpacePoseContext& Output,
	TArray<FBoneTransform>& OutBoneTransforms)
{
	const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();

	// Early out if there are no bone pairs
	if (BonePairs.Num() == 0 || Output.AnimInstanceProxy == nullptr)
	{
		return;
	}

	// Evaluate the reference pose in component space
	FComponentSpacePoseContext RefPoseContext(Output.AnimInstanceProxy);
	// Update the reference pose to ensure the sequence player progresses
	ReferencePose.EvaluateComponentSpace(RefPoseContext);

	// We'll process each bone pair
	for (const FBonePair& Pair : BonePairs)
	{
		const FCompactPoseBoneIndex TargetIndex = Pair.TargetBone.GetCompactPoseIndex(BoneContainer);
		const FCompactPoseBoneIndex RelativeIndex = Pair.RelativeBone.GetCompactPoseIndex(BoneContainer);

		const FTransform RefTargetCS = RefPoseContext.Pose.GetComponentSpaceTransform(TargetIndex);
		const FTransform RefRelativeCS = RefPoseContext.Pose.GetComponentSpaceTransform(RelativeIndex);

		const FTransform CurrentTargetCS = Output.Pose.GetComponentSpaceTransform(TargetIndex);
		const FTransform CurrentRelativeCS = Output.Pose.GetComponentSpaceTransform(RelativeIndex);

		// Create a local transform of the target bone relative to the relative bone in the reference pose
		FTransform LocalTransform;
		LocalTransform.SetLocation(RefRelativeCS.InverseTransformPosition(RefTargetCS.GetLocation()));
		LocalTransform.SetRotation(RefRelativeCS.InverseTransformRotation(RefTargetCS.GetRotation()));

		// Apply that local transform to the current relative bone's component-space transform to find the new target bone transform
		FTransform NewTargetCS;
		NewTargetCS.SetLocation(CurrentRelativeCS.TransformPosition(LocalTransform.GetLocation()));
		NewTargetCS.SetRotation(CurrentRelativeCS.TransformRotation(LocalTransform.GetRotation()));
		NewTargetCS.SetScale3D(CurrentTargetCS.GetScale3D());

		OutBoneTransforms.Add(FBoneTransform(TargetIndex, NewTargetCS));
	}
}
