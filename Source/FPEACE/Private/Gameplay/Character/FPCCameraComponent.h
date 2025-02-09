// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "FPCCameraComponent.generated.h"

class AFPCCharacter;
enum class ECameraMode : uint8;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class FPEACE_API UFPCCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

};
