// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FPCBlueprintFunctionLibrary.generated.h"

enum class ELocomotionDirection : uint8;
struct FDirectionalAnimSet;
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
	static FString GetAnimSequenceName(const UAnimSequence* TargetSequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GetIfAnimSequenceHasRootMotionDelta(UAnimSequence* TargetSequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GetIfAnimSequenceHasLoopingEnabled(UAnimSequence* TargetSequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GetIfEvaluatorHasValidSequence(const FSequenceEvaluatorReference& SequenceEvaluatorNode);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GetIfEvaluatorSequenceIsValidOrEqual(const FSequenceEvaluatorReference& SequenceEvaluatorNode, const UAnimSequenceBase* Sequence);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static void SetEvaluatorNormalizedTime(const FSequenceEvaluatorReference& SequenceEvaluatorNode, const float NormalizedTime);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static EAdditiveAnimationType GetAnimSequenceAdditiveType(UAnimSequence* TargetSequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool DoAnimSetsMatchDirectionally(const FDirectionalAnimSet& Rhs, const FDirectionalAnimSet& Lhs);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static UAnimSequence* GetDirectionalAnim(const FDirectionalAnimSet& AnimSet, const ELocomotionDirection& Direction);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category="FPEACE|Animation")
	static bool GripMatchesDirectional(const FDirectionalAnimSet& AnimSet);
};
