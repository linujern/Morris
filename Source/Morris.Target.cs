// Copyright Epic Morriss, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MorrisTarget : TargetRules
{
	public MorrisTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Morris" } );
	}
}
