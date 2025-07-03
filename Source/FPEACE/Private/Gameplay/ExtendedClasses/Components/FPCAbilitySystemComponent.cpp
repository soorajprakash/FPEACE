// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCAbilitySystemComponent.h"


// Sets default values for this component's properties
UFPCAbilitySystemComponent::UFPCAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UFPCAbilitySystemComponent::RemoveAllChildTagsOf(const FGameplayTag& ParentTag)
{
	// Get all the owned gameplay tags in a container
	FGameplayTagContainer OwnedGameplayTags;
	GetOwnedGameplayTags(OwnedGameplayTags);

	//Create a collection of tags to remove by go through the owned tags and matching them with the parent tag
	FGameplayTagContainer TagsToRemove;
	for (const FGameplayTag& OwnedTag : OwnedGameplayTags)
		if (OwnedTag.MatchesTag(ParentTag))
			TagsToRemove.AddTag(OwnedTag);
	// Add the owner to the collection
	TagsToRemove.AddTag(ParentTag);

	RemoveLooseGameplayTags(TagsToRemove);
}
