// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "UAnimGraphNode_CopyBoneToCurrentPose.h"

#define LOCTEXT_NAMESPACE "FPEACEEditor"

FText UAnimGraphNode_CopyBoneToCurrentPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("CopyBoneToCurrentPose", "CopyBoneToCurrentPose");
}

FText UAnimGraphNode_CopyBoneToCurrentPose::GetTooltipText() const
{
	return LOCTEXT("CopyBoneToCurrentPose", "Copies the allowed bone transform properties from the reference pose to the current pose.");
}


FString UAnimGraphNode_CopyBoneToCurrentPose::GetNodeCategory() const
{
	return TEXT("Skeletal Controls");
}

#undef LOCTEXT_NAMESPACE
