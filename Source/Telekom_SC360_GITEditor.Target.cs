// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Telekom_SC360_GITEditorTarget : TargetRules
{
	public Telekom_SC360_GITEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        
		ExtraModuleNames.AddRange( new string[] { "Telekom_SC360", "Telekom_SC360_Editor" } );
	}
}
