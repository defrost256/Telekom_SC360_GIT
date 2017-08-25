// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficManager.h"

// Sets default values
ATrafficManager::ATrafficManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATrafficManager::KillPercentOfAllCars(float killPercent)
{
	for (ATrafficEmitter* e : emitters)
		e->KillPercentOfCars(killPercent);
}

void ATrafficManager::ScaleAllEmitterRate(float scale)
{
	for (ATrafficEmitter* e : emitters)
	{
		e->spawnFrequency *= scale;
		e->spawnVariance *= scale;
	}
}

// Called when the game starts or when spawned
void ATrafficManager::BeginPlay()
{
	Super::BeginPlay();

	//Adds all the emitters to the traffic system, and adds their tick to the callback
	for (ATrafficEmitter* e : emitters) {
		//Add their root path to the graph
		roads.Add(e->rootRoad);
		e->sensorPool = sensorPool;
	}
	int i = 0;
	//Do a DFS and add all the child roads to the graph (in order of the DFS)
	while (i < roads.Num())
	{
		ATrafficRoad* currentRoad = roads[i];
		if (!currentRoad->IsLeaf())
		{
			for (int j = 0; j < currentRoad->childRoads.Num(); j++)
				if (j > 0)
					currentRoad->childRoads[j]->roadWeight += currentRoad->childRoads[j - 1]->roadWeight;
			float sum = currentRoad->childRoads.Last()->roadWeight;
			for (ATrafficRoad* p : currentRoad->childRoads) {
				if (!roads.Contains(p)) {
					roads.Add(p);
					p->roadWeight /= sum;
				}
			}
			currentRoad->childRoads.Last()->roadWeight = 1;
		}
		currentRoad->Initialize();
		i++;
	}
	for (ATrafficEmitter* e : emitters)
		e->Initialize();
	
}

// Called every frame
void ATrafficManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (ATrafficEmitter* emitter : emitters)
	{
		emitter->FTrafficTick(DeltaTime);
	}
	for (ATrafficRoad* road : roads)
	{
		road->FTrafficTick(DeltaTime);
	}
	sensorPoolUpdateTime += DeltaTime;
	if (sensorPoolUpdateTime >= sensorPoolUpdateRate)
	{
		sensorPool->UpdateSensors();
		sensorPoolUpdateTime -= sensorPoolUpdateRate;
	}	
}

