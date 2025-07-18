// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPCCharacterMovementComponent.generated.h"


UCLASS()
class FPEACE_API UFPCCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPCCharacterMovementComponent();

protected:

	UPROPERTY(BlueprintReadOnly)
	FVector CharacterAcceleration2D = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector CharacterVelocity2D = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	float CharacterVelocityZ = 0;
	
	UPROPERTY(BlueprintReadOnly)
	float CharacterAbsoluteSpeed = 0;
	
	UPROPERTY(BlueprintReadOnly)
	float CharacterAbsoluteSpeed2D = 0;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentVelocityAccelerationDot = 0;

	UPROPERTY(BlueprintReadOnly)
	float CharacterNormalizedSpeed = 0;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterMoving = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsCharacterAccelerating = false;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
