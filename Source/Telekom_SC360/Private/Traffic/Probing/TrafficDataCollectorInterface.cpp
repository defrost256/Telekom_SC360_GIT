// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficDataCollectorInterface.h"


// Add default functionality here for any ITrafficDataCollectorInterface functions that are not pure virtual.
FTrafficData ITrafficDataCollectorInterface::GetTrafficData()
{
	return FTrafficData();
}

FTrafficData::FTrafficData()
{
}

void FTrafficData::AddDebugTimeData(FVector loc, float time)
{
	Location_DebugTime.Add(loc);
	Time_DebugTime.Add(time);
	DataCount_DebugTime++;
}
