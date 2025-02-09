// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

// Forward declarations to reduce unnecessary includes
class USkeleton;
struct FBoneContainer;

#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "FAnimNode_CopyBoneToCurrentPose.generated.h"

// A struct to hold the target bone and the properties to copy from the reference pose
USTRUCT(BlueprintInternalUseOnly)
struct FPEACE_API FBoneTarget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Settings")
	FBoneReference TargetBone;

	UPROPERTY(EditAnywhere, Category="Settings")
	bool copyTranslation = false;

	UPROPERTY(EditAnywhere, Category="Settings")
	bool copyRotation = false;

	UPROPERTY(EditAnywhere, Category="Settings")
	bool copyScale = false;

	UPROPERTY(EditAnywhere, Category="Settings")
	bool X = false;

	UPROPERTY(EditAnywhere, Category="Settings")
	bool Y = false;

	UPROPERTY(EditAnywhere, Category="Settings")
	bool Z = false;
};

USTRUCT(BlueprintInternalUseOnly)
struct FPEACE_API FAnimNode_CopyBoneToCurrentPose final : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

	// Array of bones to copy from the reference pose to the current pose
	UPROPERTY(EditAnywhere, Category="Settings")
	TArray<FBoneTarget> TargetBones;

	// The Reference Pose input (to copy bone transform from)
	UPROPERTY(EditAnywhere, Category="Links", meta=(PinShownByDefault))
	FComponentSpacePoseLink SourcePose;

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
