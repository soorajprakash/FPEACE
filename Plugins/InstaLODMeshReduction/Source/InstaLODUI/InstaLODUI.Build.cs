/**
 * InstaLODUI.Build.cs (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODUI.Build.cs
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

using UnrealBuildTool;
using System;
using System.Reflection;
using System.Globalization;

public class InstaLODUI : ModuleRules
{
	public InstaLODUI(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivatePCHHeaderFile = "Private/InstaLODUIPCH.h";

		PrivateIncludePaths.AddRange(new string[] {
			"InstaLODUI/Private",
            "InstaLODMeshReduction/Private"
		});		
		
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
            "LevelEditor",              // Need the LevelEditor to add Toolbar Extensions and such things
            "InstaLODMeshReduction",    // Include the Base Module so we can use Styles and the API
            "Persona",                  // Need the PersonaModule for Extensions
            "StaticMeshEditor",         // Need the MeshEditor for Extensions
            "SkeletalMeshEditor",       // Need the SkeletalMeshEditor for Extensions
            "InputCore",                // ListView requires FKey, which requires this
            "RawMesh",                  // Raw Mesh Support
            "RenderCore",               // For Operations
            "RHI",                      // For Operations
            "AssetRegistry",            // To save new Assets
            "AssetTools",               // To duplicate Assets
            "MaterialUtilities",
			"StaticMeshDescription",
            "MeshDescription",
			"MaterialBaking"
        });
		
		PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
			"Slate",
			"SlateCore",
			"UnrealEd",
			"PropertyEditor",
			"ToolMenus",
			"EditorStyle",
			"SkeletalMeshUtilitiesCommon",
			"WorkspaceMenuStructure",	// For the menu
			"InstaLODMeshReduction",    // Include the Base Module so we can use Styles and the API
            "RHI",                      // For Operations
			"MeshUtilities",			// FRawMesh handling
			"MeshMergeUtilities",		// MeshMergeHelpers
            "MaterialUtilities",
			"Json",						// Profile loading
			"DesktopPlatform",			// File opening
            "MeshDescriptionOperations"
        });

		PrivateIncludePathModuleNames.AddRange(new string[] {
            "InstaLODMeshReduction",
            "StaticMeshDescription",
			"MeshMergeUtilities",
			"MaterialUtilities"
        });

		PublicIncludePaths.AddRange(new string[] {
				System.IO.Path.Combine(GetModuleDirectory("MeshMergeUtilities"), "Private"),
			});
	}
}
