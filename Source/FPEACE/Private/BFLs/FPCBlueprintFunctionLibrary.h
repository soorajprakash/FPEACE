// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FPCBlueprintFunctionLibrary.generated.h"

struct FSequenceEvaluatorReference;
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
	static bool GetIfEvaluatorHasValidSequence(const FSequenceEvaluatorReference& SequenceEvaluatorNode);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GetIfEvaluatorSequenceIsValidOrEqual(const FSequenceEvaluatorReference& SequenceEvaluatorNode, const UAnimSequenceBase* Sequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static EAdditiveAnimationType GetAnimSequenceAdditiveType(UAnimSequence* TargetSequence);
};
