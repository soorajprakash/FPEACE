// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FPCBlueprintFunctionLibrary.generated.h"

struct FAnimNode_SequenceEvaluator;
/**
 * 
 */
UCLASS()
class UFPCBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GetIfAnimSequenceHasRootMotionDelta(UAnimSequence* TargetSequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GetIfAnimSequenceHasLoopingEnabled(UAnimSequence* TargetSequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GetIfEvaluatorHasValidSequence(const FAnimNode_SequenceEvaluator& SequenceEvaluatorNode);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GetIfEvaluatorSequenceIsValidOrEqual(const FAnimNode_SequenceEvaluator& SequenceEvaluatorNode, const UAnimSequenceBase* Sequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static EAdditiveAnimationType GetAnimSequenceAdditiveType(UAnimSequence* TargetSequence);
};
