// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPCAnimInstance.generated.h"

enum class ECameraMode : uint8;
enum ELocomotionState : uint8;
class UFPCCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class FPEACE_API UFPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCCharacterMovementComponent> OwningCharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterVelocity;

	UPROPERTY(BlueprintReadOnly)
	float CharacterSpeed;

	UPROPERTY(BlueprintReadOnly)
	FVector2D CharacterVelocity2D;

	UPROPERTY(BlueprintReadOnly)
	float CharacterSpeed2D;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterMoving;

	/*
	 * Reference to the main anim instance to which this instance is added to as a layer
	 * This will be a reference to self on the main anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCAnimInstance> MainAnimInstance;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ELocomotionState> currentLocomotionState;

	static TSoftClassPtr<UFPCAnimInstance> GetAnimClassFor(const UObject* WorldContextObject, ECameraMode targetCameraMode, FName animStateName, const FString& reasonForGettingThisAnim);

protected:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
