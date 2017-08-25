// Fill out your copyright notice in the Description page of Project Settings.

#include "CarRaytraceSensor.h"

#include "TrafficCar.h"

ACarRaytraceSensor::ACarRaytraceSensor()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Sensor")));
}

float ACarRaytraceSensor::GetAvgSpeedOfOverlapCars()
{
	SCOPE_CYCLE_COUNTER(STAT_CollisionQueries);
	float acc = 0.0f;
	int cars = 0;
	for (ATrafficCar* car : overlappedCars)
	{
		acc += car->speed;
		cars++;
	}
	return acc / cars;
}

void ACarRaytraceSensor::ChangeSensorDirection(float deltaYaw)
{
	SetActorRelativeRotation(FRotator(0, deltaYaw, 0).Quaternion());
}

void ACarRaytraceSensor::GetOverlappingCars(TSet<AActor*>& outActors, TSubclassOf<AActor> classFilter, bool emergency)
{
	if (emergency)
	{
		for (ATrafficCar* car : emergencyCars)
		{
			if (!classFilter || car->IsA(classFilter))
				outActors.Add(car);
		}
	}
	for (ATrafficCar* car : overlappedCars)
	{
		if(!classFilter || car->IsA(classFilter))
			outActors.Add(car);
	}
}

bool ACarRaytraceSensor::IsOverlapping()
{
	return overlappedCars.Num() > 0;
}

bool ACarRaytraceSensor::IsEmergency()
{
	return emergencyCars.Num() > 0;
}

void ACarRaytraceSensor::AssignCar(ATrafficCar * car)
{
	Super::AssignCar(car);
}

void ACarRaytraceSensor::RemoveCar()
{
	Super::RemoveCar();
}

void ACarRaytraceSensor::BeginPlay()
{
	Super::BeginPlay();

	halfAngle = viewAngle / 2;
	deltaRotation = FRotator(0.0f, viewAngle / rayCount - 1, 0.0f);

}

void ACarRaytraceSensor::Tick(float DeltaTime)
{
	overlappedCars.Reset();
	emergencyCars.Reset();
	otherOverlaps.Reset();
	UWorld* world = GetWorld();
	FVector loc = GetActorLocation();
	FRotator rot = GetActorRotation() - FRotator(0, halfAngle + deltaRotation.Yaw, 0);
	for (int i = 0; i < rayCount; i++)
	{
		rot += deltaRotation;
		TArray<FHitResult> results;
		if(world->LineTraceMultiByChannel(results, loc, loc + rot.Vector() * maxRadius, COLLISION_CHANNEL_TRAFFIC_TRANSMIT))
		{
			FHitResult result = results[0];
			ATrafficCar* resultCar = Cast<ATrafficCar>(result.Actor.Get());
			if (result.Distance < emergencyRadius)
			{
				if (resultCar && !emergencyCars.Contains(resultCar))
				{
					emergencyCars.Add(resultCar);
				}
				else if (!resultCar && otherOverlaps.Contains(result.Actor.Get()))
				{
					otherOverlaps.Add(result.Actor.Get());
				}
			}
			else if (resultCar && !overlappedCars.Contains(resultCar))
			{
				overlappedCars.Add(resultCar);
			}
		}
	}
}
