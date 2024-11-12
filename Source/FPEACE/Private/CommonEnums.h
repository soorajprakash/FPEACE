// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

/*
 * Supported Camera mode types for the game
 */
UENUM()
enum class ECameraMode: uint8
{
    TPS,
    FPS
};

/*
 * Different states of movement the character can be in
 */
UENUM(BlueprintType)
enum ELocomotionState : uint8
{
    Walking,
    Running,
    TacSprinting
};