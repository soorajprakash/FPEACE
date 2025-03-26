// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FAnimNode_CopyBoneToCurrentPose.h"
#include "Animation/AnimInstanceProxy.h"

void FAnimNode_CopyBoneToCurrentPose::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	Super::Initialize_AnyThread(Context);
	SourcePose.Initialize(Context);
}

void FAnimNode_CopyBoneToCurrentPose::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	Super::CacheBones_AnyThread(Context);
	SourcePose.CacheBones(Context);
	for (FBoneTarget& Target : TargetBones)
		Target.TargetBone.Initialize(Context.AnimInstanceProxy->GetRequiredBones());
}

bool FAnimNode_CopyBoneToCurrentPose::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	// Validate that all bone pairs are valid
	for (const FBoneTarget& Target : TargetBones)
	{
		if (!Target.TargetBone.IsValidToEvaluate(RequiredBones))
		{
			return false;
		}
	}
	return TargetBones.Num() > 0;
}

void FAnimNode_CopyBoneToCurrentPose::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	for (FBoneTarget& Target : TargetBones)
		Target.TargetBone.Initialize(RequiredBones);
}

void FAnimNode_CopyBoneToCurrentPose::UpdateInternal(const FAnimationUpdateContext& Context)
{
	FAnimNode_SkeletalControlBase::UpdateInternal(Context);
	SourcePose.Update(Context);
}

void FAnimNode_CopyBoneToCurrentPose::EvaluateSkeletalControl_AnyThread(
	FComponentSpacePoseContext& Output,
	TArray<FBoneTransform>& OutBoneTransforms)
{
	const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();

	// Early out if there are no bone pairs
	if (TargetBones.Num() == 0 || Output.AnimInstanceProxy == nullptr)
	{
		return;
	}

	// Evaluate the reference pose in component space
	FComponentSpacePoseContext RefPoseContext(Output.AnimInstanceProxy);
	// Update the reference pose to ensure the sequence player progresses
	SourcePose.EvaluateComponentSpace(RefPoseContext);

	// We'll process each bone pair
	for (const FBoneTarget& mBoneTarget : TargetBones)
	{
		const FCompactPoseBoneIndex TargetBoneIndex = mBoneTarget.TargetBone.GetCompactPoseIndex(BoneContainer);

		if (TargetBoneIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bone %s not found in compact pose!"), *mBoneTarget.TargetBone.BoneName.ToString());
			continue; // Skip this bone to avoid crash
		}
		
		const FTransform CurrentTargetCS = Output.Pose.GetComponentSpaceTransform(TargetBoneIndex);
		const FTransform CurrentSourceCS = RefPoseContext.Pose.GetComponentSpaceTransform(TargetBoneIndex);

		// Apply the transform of the bone from the source pose to the current pose 
		FTransform NewTargetCS = CurrentTargetCS;

		if (mBoneTarget.copyTranslation)
		{
			float X = mBoneTarget.X ? CurrentSourceCS.GetLocation().X : NewTargetCS.GetLocation().X;
			float Y = mBoneTarget.Y ? CurrentSourceCS.GetLocation().Y : NewTargetCS.GetLocation().Y;
			float Z = mBoneTarget.Z ? CurrentSourceCS.GetLocation().Z : NewTargetCS.GetLocation().Z;
			NewTargetCS.SetLocation(FVector(X, Y, Z));
		}

		if (mBoneTarget.copyRotation)
		{
			float X = mBoneTarget.X ? CurrentSourceCS.GetRotation().Rotator().Pitch : NewTargetCS.GetRotation().Rotator().Pitch;
			float Y = mBoneTarget.Y ? CurrentSourceCS.GetRotation().Rotator().Yaw : NewTargetCS.GetRotation().Rotator().Yaw;
			float Z = mBoneTarget.Z ? CurrentSourceCS.GetRotation().Rotator().Roll : NewTargetCS.GetRotation().Rotator().Roll;
			NewTargetCS.SetRotation(FRotator(X, Y, Z).Quaternion());
		}

		if (mBoneTarget.copyScale)
		{
			float X = mBoneTarget.X ? CurrentSourceCS.GetScale3D().X : NewTargetCS.GetScale3D().X;
			float Y = mBoneTarget.Y ? CurrentSourceCS.GetScale3D().Y : NewTargetCS.GetScale3D().Y;
			float Z = mBoneTarget.Z ? CurrentSourceCS.GetScale3D().Z : NewTargetCS.GetScale3D().Z;
			NewTargetCS.SetScale3D(FVector(X, Y, Z));
		}

		OutBoneTransforms.Add(FBoneTransform(TargetBoneIndex, NewTargetCS));
	}
}