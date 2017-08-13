// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficSignGroup.h"

void UTrafficSignGroup::Initialize(FTrafficSignGroupParam params)
{
	DelayBetweenSwitch = params.DelayBetweenSwitch;
	DelayBeforeSwitch = params.DelayBeforeSwitch;
	currentPhase = params.startingPhase;
	if (params.startingPhase == ETrafficSignGroupPhase::None) {
		nextPhase = ETrafficSignGroupPhase::PhaseOne;
		timer = -DelayBeforeSwitch;
	}
	else
	{
		nextPhase = ETrafficSignGroupPhase::None;
		timer = 0;
	}
	phaseOneSign = params.phaseOneSign;
	phaseTwoSign = params.phaseTwoSign;
}
