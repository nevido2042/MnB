// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MnB : ModuleRules
{
	public MnB(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });

        PublicIncludePaths.Add("MnB");
    }
}
