// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectRPG : ModuleRules
{
	public ProjectRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" , "UMG", "AIModule", "NavigationSystem",
        "QuestSystemRuntime"
        });

		if (Target.bBuildEditor)
		{
             PublicDependencyModuleNames.AddRange(new string[] { "QuestSystemEditor" });

        }
    }
}
