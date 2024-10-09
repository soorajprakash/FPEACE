// Fill out your copyright notice in the Description page of Project Settings.

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
