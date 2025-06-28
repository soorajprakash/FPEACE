// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPCOperatorAnimInstance.generated.h"

class AFPCOperator;
class UFPCOperatorAnimationManagerComponent;
class UFPCOperatorCameraManagerComponent;
class UFPCOperatorWeaponManagerComponent;
class UFPCOperatorMovementComponent;
class UFPCSkeletalMeshComponent;
/**
 * 
 */
UCLASS()
class FPEACE_API UFPCOperatorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/*
	 * Reference to the character that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AFPCOperator> OwningOperator;

	/*
	 * Reference to the skeletal mesh that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCSkeletalMeshComponent> OwningMesh;

	/*
	 * Reference to the movement component in the character that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCOperatorMovementComponent> OwningCharacterMovementComponent;

	/*
	 * Reference to the weapon manager component in the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCOperatorWeaponManagerComponent> OwningCharacterWeaponManager;

	/*
	 * Reference to the camera manager component in the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCOperatorCameraManagerComponent> OwningCharacterCameraManager;

	/*
	 * Reference to the animation manager component in the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCOperatorAnimationManagerComponent> OwningCharacterAnimationManager;

protected:
	virtual void NativeInitializeAnimation() override;

	/*
	 * Used to rename the anim instance object so that it's clearer when debugging in the editor
	 */
	UFUNCTION(BlueprintCallable)
	void RenameAnimInstanceObject(const FString& NewName);

private:
	void InitializeReferences();
};
