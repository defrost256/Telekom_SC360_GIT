// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/Components/PrimitiveComponent.h"

#ifndef COLLISION_CHANNEL_TRAFFIC_RECEIVE
#define COLLISION_CHANNEL_TRAFFIC_RECEIVE ECollisionChannel::ECC_GameTraceChannel1
#endif // !COLLISION_CHANNEL_TRAFFIC_RECEIVE

#ifndef COLLISION_CHANNEL_TRAFFIC_TRANSMIT
#define COLLISION_CHANNEL_TRAFFIC_TRANSMIT ECollisionChannel::ECC_GameTraceChannel2
#endif // !COLLISION_CHANNEL_TRAFFIC_TRANSMIT

DECLARE_STATS_GROUP(TEXT("TrafficSystem"), STATGROUP_TrafficSystem, STATCAT_Advanced);
DECLARE_CYCLE_STAT_EXTERN(TEXT("Traffic Tick"), STAT_TrafficTick, STATGROUP_TrafficSystem, TELEKOM_SC360_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("Collision Queries"), STAT_CollisionQueries, STATGROUP_TrafficSystem, TELEKOM_SC360_API);
DECLARE_DWORD_COUNTER_STAT_EXTERN(TEXT("Number of Cars"), STAT_CarCounter, STATGROUP_TrafficSystem, TELEKOM_SC360_API);

/**
 * 
 */
class TELEKOM_SC360_API TrafficHelper
{
public:
	TrafficHelper();
	~TrafficHelper();

	static void SetTrafficTransmitterCollision(UPrimitiveComponent* component);

	static void SetTrafficReceiverCollision(UPrimitiveComponent* component);
};
