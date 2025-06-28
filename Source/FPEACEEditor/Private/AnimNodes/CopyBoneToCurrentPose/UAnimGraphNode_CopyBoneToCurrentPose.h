// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "AnimGraphNode_SkeletalControlBase.h"
#include "AnimNodes/CopyBoneToCurrentPose/FAnimNode_CopyBoneToCurrentPose.h"
#include "UAnimGraphNode_CopyBoneToCurrentPose.generated.h"

UCLASS()
class FPEACEEDITOR_API UAnimGraphNode_CopyBoneToCurrentPose : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Settings)
	FAnimNode_CopyBoneToCurrentPose Node;

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }

protected:
	virtual FString GetNodeCategory() const override;
};
