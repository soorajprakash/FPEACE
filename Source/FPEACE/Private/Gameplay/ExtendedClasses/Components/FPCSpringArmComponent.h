// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "FPCSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class FPEACE_API UFPCSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	void SetArmLengthFromPitch(const UCurveFloat* PitchToArmLengthCurve, const FRotator& ControllerRotation);
};
