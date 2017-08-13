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
