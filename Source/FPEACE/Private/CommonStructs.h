// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CommonStructs.generated.h"

class UAnimSequence;

/*
 * A Collection of animation sequences used for a cycle animation directionality
 */
USTRUCT(BlueprintType)
struct FCycleAnimSet
{
	GENERATED_BODY()

	FCycleAnimSet(): Forward(nullptr), Backward(nullptr), Right(nullptr), Left(nullptr)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Backward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Left;
};

USTRUCT(BlueprintType)
struct FCharacterDirectionAngleLimits
{
	GENERATED_BODY()

	FCharacterDirectionAngleLimits(): ForwardLimits(FVector2D::Zero()), BackwardLimits(FVector2D::Zero()), DirectionalDeadzone(0)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D ForwardLimits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D BackwardLimits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DirectionalDeadzone;
};

/*
 * Struct containing settings for different types of movement modes like Walking, running etc
 */
USTRUCT()
struct FLocomotionStateSetting
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly)
	float MaxAcceleration;

	UPROPERTY(EditDefaultsOnly)
	float BrakingDeceleration;

	UPROPERTY(EditDefaultsOnly)
	float BrakingFrictionFactor;

	UPROPERTY(EditDefaultsOnly)
	float BrakingFriction;

	UPROPERTY(EditDefaultsOnly)
	bool bUseSeparateBrakingFriction;

	// Constructor to set initial values
	FLocomotionStateSetting()
		: MaxWalkSpeed(0), MaxAcceleration(0), BrakingDeceleration(0), BrakingFrictionFactor(0), BrakingFriction(0), bUseSeparateBrakingFriction(false)
	{
	}
};
