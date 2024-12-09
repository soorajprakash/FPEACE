#pragma once

#include "AnimGraphNode_SkeletalControlBase.h"
#include "FAnimNode_SetRelativeTransformFromPose.h"
#include "UAnimGraphNode_SetRelativeTransformFromPose.generated.h"

UCLASS()
class FPEACE_API UAnimGraphNode_SetRelativeTransformFromPose : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Settings)
	FAnimNode_SetRelativeTransformFromPose Node;

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;

	virtual const FAnimNode_SkeletalControlBase* GetNode() const override;

protected:
	virtual FString GetNodeCategory() const override;
};
