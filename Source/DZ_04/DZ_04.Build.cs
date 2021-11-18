// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DZ_04 : ModuleRules
{
	public DZ_04(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
