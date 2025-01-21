// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPCAnimInstance.generated.h"

class UFPCSkeletalMeshComponent;
class UFPCCharacterData;
enum class ELocomotionDirection : uint8;
enum class ELocomotionState : uint8;
class AFPCCharacter;
enum class ECameraMode : uint8;
class UFPCCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class FPEACE_API UFPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/*
	 * This is used to only perform operations on the base anim instance for optimization
	 */
	UPROPERTY(BlueprintReadWrite)
	bool isBaseAnimInstance;

	/*
	 * Reference to the character that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AFPCCharacter> OwningCharacter;

	/*
	 * Reference to the skeletal mesh that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCSkeletalMeshComponent> OwningMesh;

	/*
	 * For anim blueprints that are layered on top of the base anim instance, this value will be something other than self
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCAnimInstance> OwningAnimInstance;

	UPROPERTY(BlueprintReadOnly)
	UFPCAnimInstance* CurrentLinkedAnimInstance;

	/*
	 * Used to hold the active weapon grip pose on the owning mesh
	 */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimSequence> CurrentWeaponGripPose;

	/*
	 * Reference to the movement component in the character that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCCharacterMovementComponent> OwningCharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly)
	float LeanAngle;

	TSoftClassPtr<UFPCAnimInstance> GetAnimClassFor(ECameraMode TargetCameraMode, FName AnimStateName, const FString& ReasonForGettingThisAnim);

protected:
	/*
	 * Reference to the character data asset referenced in the game instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCCharacterData> OwningCharacterData;

	virtual void NativeBeginPlay() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

private:
	/*
	 * Used to calculate angular velocity
	 */
	float previousYaw;

	void CalculateLeanAngle(float DeltaSeconds);

	void InitializeReferences();
};
