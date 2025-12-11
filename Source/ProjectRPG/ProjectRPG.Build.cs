// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectRPG : ModuleRules
{
	public ProjectRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", 
			"SlateCore",
			"EnhancedInput" , 
			"UMG", 
			"AIModule", "NavigationSystem",
			"QuestSystemRuntime",
			"Niagara",
			"VaRest"
        });

		if (Target.bBuildEditor)
		{
             PublicDependencyModuleNames.AddRange(new string[] { "QuestSystemEditor" });
        }

        PublicDefinitions.Add("__has_feature(x)=0");
    }
}
