#include "UAnimGraphNode_SetRelativeTransformFromPose.h"

#define LOCTEXT_NAMESPACE "A3Nodes"

FText UAnimGraphNode_SetRelativeTransformFromPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("Set Relative Transform From Pose", "Set Relative Transform From Pose");
}

FText UAnimGraphNode_SetRelativeTransformFromPose::GetTooltipText() const
{
	return LOCTEXT("Set Relative Transform From Pose", "Calculates the relative transform between two bones from a reference pose and applies it to the bone in the current pose");
}

const FAnimNode_SkeletalControlBase* UAnimGraphNode_SetRelativeTransformFromPose::GetNode() const
{
	// Return a pointer to the runtime node which inherits from FAnimNode_SkeletalControlBase
	return &Node;
}


FString UAnimGraphNode_SetRelativeTransformFromPose::GetNodeCategory() const
{
	return TEXT("Skeletal Controls");
}

#undef LOCTEXT_NAMESPACE
