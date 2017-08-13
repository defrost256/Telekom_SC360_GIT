// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Telekom_SC360EditorTarget : TargetRules
{
	public Telekom_SC360EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        
		ExtraModuleNames.AddRange( new string[] { "Telekom_SC360" } );
	}
}
