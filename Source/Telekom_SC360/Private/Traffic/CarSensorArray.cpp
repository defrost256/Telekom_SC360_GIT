// Fill out your copyright notice in the Description page of Project Settings.

#include "CarSensorArray.h"

float ACarSensorArray::GetAvgSpeedOfOverlapCars()
{
	return 0.0f;
}

void ACarSensorArray::ChangeSensorDirection(ESensorDirection newDirection)
{
}

void ACarSensorArray::GetOverlappingCars(TSet<AActor*>& actors, TSubclassOf<AActor> classFilter)
{
}

FString ACarSensorArray::GetCurrentSensorDirectionName()
{
	return FString();
}

void ACarSensorArray::AssignCar(ATrafficCar * car)
{
	MyCar = car;
}

void ACarSensorArray::RemoveCar()
{
	MyCar = nullptr;
}

bool ACarSensorArray::IsOverlapping()
{
	return false;
}

FString ACarSensorArray::GetSensorDirectionName(ESensorDirection dir)
{
	switch (dir)
	{
	case ESensorDirection::Front:
		return TEXT("Front");
	case ESensorDirection::FrontRight:
		return TEXT("FrontRight");
	case ESensorDirection::FrontLeft:
		return TEXT("FrontLeft");
	case ESensorDirection::Right:
		return TEXT("Right");
	case ESensorDirection::Left:
		return TEXT("Left");
	case ESensorDirection::Rear:
		return TEXT("Rear");
	default:
		return TEXT("N/A");
	}
	return TEXT("N/A");
}

// Sets default values
ACarSensorArray::ACarSensorArray()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACarSensorArray::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACarSensorArray::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

