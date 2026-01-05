// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Zeltina_UnrealGASEditorTarget : TargetRules
{
	public Zeltina_UnrealGASEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Zeltina_UnrealGAS" } );
	}
}
