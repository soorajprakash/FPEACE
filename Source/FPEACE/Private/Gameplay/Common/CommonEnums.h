// Copyright © Sooraj Prakash. All rights reserved.
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
	Stationary,
	Walking,
	Running,
	Sprinting
};

/*
 * Different states of character stances
 */
UENUM(BlueprintType)
enum class ELocomotionStance : uint8
{
	Standing,
	Crouching,
	Proning
};

UENUM(BlueprintType)
enum class ELocomotionDirection : uint8
{
	Center,
	Forward,
	Backward,
	Right,
	Left
};

UENUM(BlueprintType)
enum class ELocomotionAnimType : uint8
{
	Idle,
	Start,
	Stop,
	Pivot,
	Cycle
};
