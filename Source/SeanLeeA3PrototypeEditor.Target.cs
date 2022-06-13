// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SeanLeeA3PrototypeEditorTarget : TargetRules
{
	public SeanLeeA3PrototypeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("SeanLeeA3Prototype");
	}
}
