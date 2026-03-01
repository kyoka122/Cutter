// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cutter : ModuleRules
{
	public Cutter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate",
			"SlateCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Cutter",
			"Cutter/Variant_Strategy",
			"Cutter/Variant_Strategy/UI",
			"Cutter/Variant_TwinStick",
			"Cutter/Variant_TwinStick/AI",
			"Cutter/Variant_TwinStick/Gameplay",
			"Cutter/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
