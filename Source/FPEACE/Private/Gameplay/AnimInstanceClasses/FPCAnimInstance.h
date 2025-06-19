// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPCAnimInstance.generated.h"

class UFPCCharacterAnimationManagerComponent;
class UFPCCharacterCameraManagerComponent;
class UFPCCharacterWeaponManagerComponent;
class UFPCCharacterMovementComponent;
class UFPCSkeletalMeshComponent;
class AFPCCharacter;
/**
 * 
 */
UCLASS()
class FPEACE_API UFPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/*
	 * Reference to the character that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AFPCCharacter> OwningCharacter;

	/*
	 * Reference to the skeletal mesh that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCSkeletalMeshComponent> OwningMesh;

	/*
	 * Reference to the movement component in the character that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCCharacterMovementComponent> OwningCharacterMovementComponent;

	/*
	 * Reference to the weapon manager component in the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCCharacterWeaponManagerComponent> OwningCharacterWeaponManager;

	/*
	 * Reference to the camera manager component in the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCCharacterCameraManagerComponent> OwningCharacterCameraManager;

	/*
	 * Reference to the animation manager component in the owning character
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UFPCCharacterAnimationManagerComponent> OwningCharacterAnimationManager;

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
