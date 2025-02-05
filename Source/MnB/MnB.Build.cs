// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildBase;
using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class MnB : ModuleRules
{
	public MnB(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", 
			"UMG", 
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"HeadMountedDisplay",
            "XRBase",
            "AnimGraphRuntime",
            "MoviePlayer"
        });

		if(Target.Type == TargetType.Editor)
		{
			PublicDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd"
			}
			);

        }

        PublicIncludePaths.Add("MnB");
        PublicIncludePaths.Add("MnB/Interface");
    }
}
