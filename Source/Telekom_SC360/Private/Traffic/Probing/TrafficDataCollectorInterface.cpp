// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficDataCollectorInterface.h"


// Add default functionality here for any ITrafficDataCollectorInterface functions that are not pure virtual.
const FTrafficData* ITrafficDataCollectorInterface::GetTrafficData() const
{
	return nullptr;
}

FTrafficData::FTrafficData()
{
}

void FTrafficData::AddDebugTimeData(FVector loc, float time, float speed)
{
	Location_SpeedCurve.Add(loc);
	Time_SpeedCurve.Add(time);
	Speed_SpeedCurve.Add(speed);
	DataCount_DebugTime++;
}

void FTrafficData::SetColorOf(ETrafficDataType type, FColor color)
{
	switch (type)
	{
	case ETrafficDataType::None:
		break;
	case ETrafficDataType::SpeedCurve:
		Color_SpeedCurve = color;
		break;
	default:
		break;
	}
}
