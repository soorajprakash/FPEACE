// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

/*
 * Supported Camera mode types for the game
 */
UENUM(BlueprintType)
enum class ECameraMode: uint8
{
	TPS,
	FPS
};

/*
 * Different states of movement the character can be in
 */
UENUM(BlueprintType)
enum class ELocomotionState : uint8
{
	Walking,
	Running,
	Sprinting
};

UENUM(BlueprintType)
enum class ELocomotionDirection : uint8
{
	Forward,
	Backward,
	Right,
	Left
};

UENUM(BlueprintType)
enum class EEvaluatorAnimType : uint8
{
	Start,
	Stop,
	Pivot
};
