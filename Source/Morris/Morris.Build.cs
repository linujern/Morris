// Copyright Epic Morriss, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Morris : ModuleRules
{
	public Morris(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks", "AIModule", "NavigationSystem", "Navmesh"  });

		PrivateDependencyModuleNames.AddRange(new string[] { "PrefabricatorRuntime", "Niagara" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
