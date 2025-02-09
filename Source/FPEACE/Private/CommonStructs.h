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

	FCycleAnimSet(): WeaponHandGripPose(nullptr), bAreDirectionalAnimsAdditive(false), bSyncUpperAndLowerBodyPlayers(true), Forward(nullptr), Backward(nullptr), Right(nullptr), Left(nullptr)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> WeaponHandGripPose;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAreDirectionalAnimsAdditive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSyncUpperAndLowerBodyPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Backward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Left;
};

/*
 * Used to define the angle limits for a character's direction change registration
 */
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

/*
 * Used to hold the camera settings for different camera modes
 */
USTRUCT()
struct FCharacterCameraModeSettings
{
	GENERATED_BODY()

	// Constructor to set initial values
	FCharacterCameraModeSettings(): SpringArmTramsformOffset(), SpringArmSocketOffset(), SpringArmTargetOffset(), MaxSpringArmLength(0), bUsePawnControlRotation(false), bEnableCameraLag(false),
	                                CameraLagSpeed(0),
	                                CameraLagMaxDistance(0)
	{
	}

	UPROPERTY(EditDefaultsOnly)
	FVector SpringArmTramsformOffset;

	UPROPERTY(EditDefaultsOnly)
	FVector SpringArmSocketOffset;

	UPROPERTY(EditDefaultsOnly)
	FVector SpringArmTargetOffset;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpringArmLength;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> PitchToArmLengthCurve;

	UPROPERTY(EditDefaultsOnly)
	bool bUsePawnControlRotation;

	UPROPERTY(EditDefaultsOnly)
	bool bEnableCameraLag;

	UPROPERTY(EditDefaultsOnly)
	float CameraLagSpeed;

	UPROPERTY(EditDefaultsOnly)
	float CameraLagMaxDistance;
};
