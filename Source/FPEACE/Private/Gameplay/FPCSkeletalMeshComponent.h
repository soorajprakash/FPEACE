// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "FPCSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class FPEACE_API UFPCSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UFPCSkeletalMeshComponent();
};
