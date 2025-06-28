// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FPCCharacterAnimationStateChangedNotify.generated.h"

UENUM(BlueprintType)
enum ENotifyAnimationType
{
	EnterADS,
	ExitADS
};

UENUM(BlueprintType)
enum ENotifyAnimationEventType
{
	Started,
	Ended
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimationStateChangeNotify, ENotifyAnimationType, AnimationType, ENotifyAnimationEventType, AnimationEventType);

/**
 * 
 */
UCLASS(BlueprintType)
class UFPCCharacterAnimationStateChangedNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	static FAnimationStateChangeNotify OnAnimationStateChanged;

protected:
	UPROPERTY(EditAnywhere, Category="FPEACE")
	TEnumAsByte<ENotifyAnimationType> AnimationType;

	UPROPERTY(EditAnywhere, Category="FPEACE")
	TEnumAsByte<ENotifyAnimationEventType> AnimationEventType;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
