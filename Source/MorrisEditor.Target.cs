// Copyright Epic Morriss, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MorrisEditorTarget : TargetRules
{
	public MorrisEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Morris" } );
	}
}
