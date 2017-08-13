// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Telekom_SC360_GITTarget : TargetRules
{
	public Telekom_SC360_GITTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Telekom_SC360" } );
    }
}
