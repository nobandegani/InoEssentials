// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class InoEditorEssentials : ModuleRules
{
	public InoEditorEssentials(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				string.Format("{0}/Public", ModuleDirectory),
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				string.Format("{0}/Private", ModuleDirectory),
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"EngineSettings",
				"AndroidRuntimeSettings",
				"DesktopPlatform"
				// ... add other public dependencies that you statically link with here ...
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				
				// ... add private dependencies that you statically link with here ...	
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
		
		if (Target.bBuildEditor == true)
		{
			//@TODO: UnrealEd Needed for the triangulation code used for sprites (but only in editor mode)
			//@TOOD: Try to move the code dependent on the triangulation code to the editor-only module
			PrivateIncludePathModuleNames.AddRange(
				new string[]
				{
					"UnrealEd",
				}
			);
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"EditorFramework",
					"UnrealEd",
				}
			);
		}
	}
}