// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCOperatorAnimInstance.h"
#include "FPCOperatorUtilityAnimInstance.generated.h"

class UFPCOperatorSkeletalAnimInstance;
/**
 * The type of anim instance used as linked anim graph instances in other anim instances
 */
UCLASS()
class FPEACE_API UFPCOperatorUtilityAnimInstance : public UFPCOperatorAnimInstance
{
	GENERATED_BODY()

public:
	/*
	 * For anim blueprints that are layered on top of the base anim instance, this value will be something other than self
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCOperatorSkeletalAnimInstance> OwningAnimInstance;

	/*
	 * Used to hold the active weapon grip pose on the owning mesh
	 * Updated at runtime in blueprint
	 */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimSequence> CurrentWeaponGripPose;

protected:

	virtual void NativeInitializeAnimation() override;
};
