// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficHelper.h"

TrafficHelper::TrafficHelper()
{
}

TrafficHelper::~TrafficHelper()
{
}

void TrafficHelper::SetTrafficTransmitterCollision(UPrimitiveComponent * component)
{
	component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	component->SetCollisionResponseToChannel(COLLISION_CHANNEL_TRAFFIC_RECEIVE, ECollisionResponse::ECR_Overlap);
	component->SetCollisionObjectType(COLLISION_CHANNEL_TRAFFIC_TRANSMIT);
	component->SetCollisionProfileName(FName(TEXT("TrafficTransmitter")));
	//component->UpdateCollisionProfile();
}

void TrafficHelper::SetTrafficReceiverCollision(UPrimitiveComponent * component)
{
	component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	component->SetCollisionResponseToChannel(COLLISION_CHANNEL_TRAFFIC_TRANSMIT, ECollisionResponse::ECR_Overlap);
	component->SetCollisionObjectType(COLLISION_CHANNEL_TRAFFIC_RECEIVE);
	component->SetCollisionProfileName(FName(TEXT("TrafficReceiver")));
	//component->UpdateCollisionProfile();
}
