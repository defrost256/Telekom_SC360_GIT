// Fill out your copyright notice in the Description page of Project Settings.

#include "CarSensorArray.h"

#include "TrafficCar.h"

float ACarSensorArray::GetAvgSpeedOfOverlapCars()
{
	return 0.0f;
}

void ACarSensorArray::ChangeSensorDirection(float delatYaw)
{

}

void ACarSensorArray::GetOverlappingCars(TSet<AActor*>& outActors, TSubclassOf<AActor> classFilter, bool emergency)
{
}

void ACarSensorArray::AssignCar(ATrafficCar * car)
{
	MyCar = car;
	car->AssignSensor(this);
	AttachToActor(car, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACarSensorArray::RemoveCar()
{
	MyCar->RemoveSensor();
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	MyCar = nullptr;
}

bool ACarSensorArray::IsOverlapping()
{
	return false;
}

bool ACarSensorArray::IsEmergency()
{
	return false;
}

bool ACarSensorArray::IsProxy()
{
	return false;
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

