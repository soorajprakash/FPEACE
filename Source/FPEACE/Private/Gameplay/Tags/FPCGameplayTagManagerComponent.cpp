// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCGameplayTagManagerComponent.h"


// Sets default values for this component's properties
UFPCGameplayTagManagerComponent::UFPCGameplayTagManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


void UFPCGameplayTagManagerComponent::RemoveGameplayTags(const FGameplayTagContainer& ContainerOfTagsToRemove)
{
	GameplayTagsContainer.RemoveTags(ContainerOfTagsToRemove);
}

void UFPCGameplayTagManagerComponent::AddGameplayTags(const FGameplayTagContainer& ContainerOfNewTags)
{
	GameplayTagsContainer.AppendTags(ContainerOfNewTags);
}

bool UFPCGameplayTagManagerComponent::HasGameplayTag(const FGameplayTag GameplayTag, bool bCheckExactMatch) const
{
	if (bCheckExactMatch)
		return GameplayTagsContainer.HasTagExact(GameplayTag);

	return GameplayTagsContainer.HasTag(GameplayTag);
}
