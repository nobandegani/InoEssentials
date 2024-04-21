// © 2024. All rights reserved by Inoland

using UnrealBuildTool;

public class InoSubsystemEditor : ModuleRules
{
    public InoSubsystemEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "AssetTools", "InoSubsystem",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InoSubsystem",
                "UnrealEd",
            }
        );
    }
}