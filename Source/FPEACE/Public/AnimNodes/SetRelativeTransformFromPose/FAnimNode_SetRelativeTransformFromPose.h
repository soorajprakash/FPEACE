// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

// Forward declarations to reduce unnecessary includes
class USkeleton;
struct FBoneContainer;

#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "FAnimNode_SetRelativeTransformFromPose.generated.h"

// A struct to hold one target-relative bone pair
USTRUCT(BlueprintInternalUseOnly)
struct FPEACE_API FBonePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Settings")
	FBoneReference TargetBone;

	UPROPERTY(EditAnywhere, Category="Settings")
	FBoneReference RelativeBone;
};

USTRUCT(BlueprintInternalUseOnly)
struct FPEACE_API FAnimNode_SetRelativeTransformFromPose final : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

	// Array of bone pairs (Target and Relative)
	UPROPERTY(EditAnywhere, Category="Settings")
	TArray<FBonePair> BonePairs;

	// The Reference Pose input (to determine the difference)
	UPROPERTY(EditAnywhere, Category="Links", meta=(PinShownByDefault))
	FComponentSpacePoseLink ReferencePose;

protected:
	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	// End of FAnimNode_Base interface

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;
	// End of FAnimNode_SkeletalControlBase interface
};
