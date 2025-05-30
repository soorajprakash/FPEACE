// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "FPCSkeletalMeshComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisibilityStateChanged, bool, bIsVisible);

/**
 * An extension of the Skeletal Mesh Component Class
 */
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class FPEACE_API UFPCSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UFPCSkeletalMeshComponent();

	UPROPERTY(BlueprintAssignable)
	FOnVisibilityStateChanged OnOwnerNoSeeStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnVisibilityStateChanged OnOnlyOwnerSeeStateChanged;

	virtual void FPC_SetOwnerNoSee(bool bNoSee);
	virtual void FPC_SetOnlyOwnerSee(bool bSee);
};