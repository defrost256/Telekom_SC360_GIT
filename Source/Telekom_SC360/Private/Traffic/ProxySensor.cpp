// Fill out your copyright notice in the Description page of Project Settings.

#include "ProxySensor.h"

AProxySensor::AProxySensor()
{
}

float AProxySensor::GetAvgSpeedOfOverlapCars()
{
	return 0.0f;
}

void AProxySensor::ChangeSensorDirection(ESensorDirection newDirection)
{
}

void AProxySensor::GetOverlappingCars(TSet<AActor*>& actors, TSubclassOf<AActor> classFilter)
{
}

FString AProxySensor::GetCurrentSensorDirectionName()
{
	return FString();
}

bool AProxySensor::IsOverlapping()
{
	return false;
}

void AProxySensor::AssignCar(ATrafficCar * car)
{
	Super::AssignCar(car);
}

void AProxySensor::RemoveCar()
{
	Super::RemoveCar();
}

void AProxySensor::BeginPlay()
{
}

void AProxySensor::Tick(float DeltaTime)
{
}
