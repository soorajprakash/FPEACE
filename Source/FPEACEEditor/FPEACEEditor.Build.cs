// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

using UnrealBuildTool;

public class FPEACEEditor : ModuleRules
{
	public FPEACEEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"AnimationModifiers",
				"AnimationBlueprintLibrary",
				"AnimGraph",
				"BlueprintGraph"
			});

			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"FPEACE" // Runtime module reference
			});
		}
	}
}