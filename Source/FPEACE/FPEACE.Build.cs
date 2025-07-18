// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

using UnrealBuildTool;

public class FPEACE : ModuleRules
{
	public FPEACE(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new[] { "FCTween", "ObjectPool" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AnimGraphRuntime",
			"AnimationLocomotionLibraryRuntime", "Niagara", "UMG", "SlateCore", "Slate", "CommonUI", "CinematicCamera",
			"ModelViewViewModel", "SignificanceManager", "GameplayTags", "GameplayAbilities", "GameplayTasks",
			"GameplayStateTreeModule", "AIModule"
		});
	}
}