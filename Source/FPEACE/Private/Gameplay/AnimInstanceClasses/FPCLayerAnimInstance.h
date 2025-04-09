// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCAnimInstance.h"
#include "FPCLayerAnimInstance.generated.h"

class AFPCWeapon;
class UFPCSkeletalAnimInstance;
/**
 * The type of anim instance class meant to be used in anim blueprints that define layer logic
 */
UCLASS()
class FPEACE_API UFPCLayerAnimInstance : public UFPCAnimInstance
{
	GENERATED_BODY()

public:
	/*
	 * For anim blueprints that are layered on top of the base anim instance, this value will be something other than self
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCSkeletalAnimInstance> OwningAnimInstance;

	/*
	 * Used to hold the active weapon grip pose on the owning mesh
	 * Updated at runtime in blueprint
	 */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimSequence> CurrentWeaponGripPose;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCurrentWeaponUsed();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponEquipped(const AFPCWeapon* SpawnedWeapon);

protected:
	virtual void NativeInitializeAnimation() override;
};
