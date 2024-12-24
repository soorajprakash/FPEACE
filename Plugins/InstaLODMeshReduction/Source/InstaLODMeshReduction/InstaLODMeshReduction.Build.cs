/**
 * InstaLODMeshReduction.Build.cs (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODMeshReduction.Build.cs
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

using UnrealBuildTool;
using System;
using System.Reflection;
using System.Globalization;

public class InstaLODMeshReduction : ModuleRules
{

	public InstaLODMeshReduction(ReadOnlyTargetRules Target) : base(Target)
	{
#if UE_5_2_OR_LATER
		IWYUSupport = IWYUSupport.None;
#else
		bEnforceIWYU = false;
#endif

		PrivatePCHHeaderFile = "InstaLODMeshReductionPCH.h";

		PublicDefinitions.Add("INSTALOD_LIB_DYNAMIC=1");


		PrivateIncludePaths.AddRange(new string[] {
			"InstaLODMeshReduction/Private",
		});

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"RenderCore",
			"RHI",
			"Slate",
			"SlateCore",
			"InputCore",
			"UnrealEd",
			"LevelEditor",
			"PropertyEditor",
			"StaticMeshEditor",
			"Persona",
			"EditorStyle",
			"StaticMeshDescription",
			"MeshDescription",
			"MeshDescriptionOperations",
			"MeshBoneReduction",
			"MeshPaint",
			"Projects",
			"AnimationBlueprintLibrary",
			"ClothingSystemRuntimeCommon",
			"SkeletalMeshUtilitiesCommon",
			"RawMesh",
			"MaterialUtilities"
		});

		PrivateIncludePathModuleNames.AddRange(new string[] {
			"Settings",
			"AssetTools",
			"MeshUtilities",
			"MaterialUtilities",
			"StaticMeshDescription",
			"MeshDescription",
			"MeshDescriptionOperations",
			"MeshBoneReduction",
			"ClothingSystemRuntimeCommon",
			"StaticMeshEditor",
			"AnimationBlueprintLibrary",
			"EditorStyle",
			"SkeletalMeshUtilitiesCommon",
			"Persona"
		});
	}
}
