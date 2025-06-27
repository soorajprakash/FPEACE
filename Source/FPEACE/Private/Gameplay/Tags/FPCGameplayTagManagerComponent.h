// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Gameplay/FPCActorComponent.h"
#include "FPCGameplayTagManagerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPEACE_API UFPCGameplayTagManagerComponent : public UFPCActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPCGameplayTagManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer GameplayTagsContainer;

	UFUNCTION(BlueprintCallable)
	void RemoveGameplayTags(const FGameplayTagContainer& ContainerOfTagsToRemove);

	UFUNCTION(BlueprintCallable)
	void AddGameplayTags(const FGameplayTagContainer& ContainerOfNewTags);

	UFUNCTION(BlueprintCallable)
	bool HasGameplayTag(const FGameplayTag GameplayTag, bool bCheckExactMatch) const;
};
