// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class X4StationBuilding : ModuleRules
{
	public X4StationBuilding(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "UMGEditor" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[]
		{
			"X4StationBuilding", 
			"X4StationBuilding/Public/Data",
			"X4StationBuilding/Public/UI",
			"X4StationBuilding/Public/Core"
		});
		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
