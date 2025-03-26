// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FPCBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UFPCBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	static bool GetIfAnimSequenceHasRootMotionDelta(UAnimSequence* TargetSequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	static bool GetIfAnimSequenceHasLoopingEnabled(UAnimSequence* TargetSequence);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	static EAdditiveAnimationType GetAnimSequenceAdditiveType(UAnimSequence* TargetSequence);
};
