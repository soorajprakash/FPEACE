// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "UAnimGraphNode_SetRelativeTransformFromPose.h"

#define LOCTEXT_NAMESPACE "FPEACEEditor"

FText UAnimGraphNode_SetRelativeTransformFromPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("Set Relative Transform From Pose", "Set Relative Transform From Pose");
}

FText UAnimGraphNode_SetRelativeTransformFromPose::GetTooltipText() const
{
	return LOCTEXT("Set Relative Transform From Pose", "Calculates the relative transform between two bones from a reference pose and applies it to the bone in the current pose");
}


FString UAnimGraphNode_SetRelativeTransformFromPose::GetNodeCategory() const
{
	return TEXT("Skeletal Controls");
}

#undef LOCTEXT_NAMESPACE
