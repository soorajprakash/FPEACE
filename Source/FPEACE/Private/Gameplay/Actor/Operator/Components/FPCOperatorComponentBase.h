// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ExtendedClasses/Components/FPCActorComponent.h"
#include "FPCOperatorComponentBase.generated.h"


class UFPCSpringArmComponent;
class UFPCOperatorAnimationManagerComponent;
class UFPCOperatorCameraManagerComponent;
class UFPCOperatorSkeletalAnimInstance;
class UFPCOperatorWeaponManagerComponent;
class UFPCOperatorMovementComponent;
class UFPCSkeletalMeshComponent;
class UFPCCharacterData;
class AFPCGameplayPlayerController;
class AFPCOperator;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPEACE_API UFPCOperatorComponentBase : public UFPCActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPCOperatorComponentBase();

protected:
	UPROPERTY()
	TWeakObjectPtr<AFPCOperator> OwningOperator;

	UPROPERTY()
	TWeakObjectPtr<AFPCGameplayPlayerController> PlayerControllerRef;

	UPROPERTY()
	TWeakObjectPtr<UFPCCharacterData> FPCOperatorData;
	
	UPROPERTY()
	TWeakObjectPtr<UFPCOperatorMovementComponent> FPCOperatorMovementComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCOperatorCameraManagerComponent> FPCOperatorCameraManagerComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCOperatorAnimationManagerComponent> FPCOperatorAnimationManagerComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCOperatorWeaponManagerComponent> FPCOperatorWeaponManagerComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCSpringArmComponent> FPCSpringArmComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCSkeletalMeshComponent> MainBodyMeshComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCSkeletalMeshComponent> FPSBodyMeshComp;

	UPROPERTY()
	TWeakObjectPtr<UFPCSkeletalMeshComponent> FPSLowerBodyMeshComp;

	/*
	 * Reference to the anim instance running on the base skeletal mesh component
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCOperatorSkeletalAnimInstance> TPSMeshAnimInstance;

	/*
	 * Reference to the anim instance running on the FPS skeletal mesh component
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCOperatorSkeletalAnimInstance> FPSMeshAnimInstance;

	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;
};
