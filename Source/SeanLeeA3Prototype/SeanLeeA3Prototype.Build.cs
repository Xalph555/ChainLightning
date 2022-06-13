// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SeanLeeA3Prototype : ModuleRules
{
	public SeanLeeA3Prototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "SlateCore", "NavigationSystem", "GameplayTasks", "Niagara" });

	}
}
