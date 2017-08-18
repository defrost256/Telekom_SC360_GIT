// Fill out your copyright notice in the Description page of Project Settings.

#include "SensorPool.h"


// Sets default values
ASensorPool::ASensorPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASensorPool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASensorPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASensorPool::AssignSensor(ATrafficCar * car)
{

}

void ASensorPool::RevokeSensor(ACarSensorArray * sensor)
{
}

