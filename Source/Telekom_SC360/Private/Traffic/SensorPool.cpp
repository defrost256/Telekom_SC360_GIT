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
	
	swapRadiusLowSq = swapRadiusLow * swapRadiusLow;
	swapRadiusHighSq = swapRadiusHigh * swapRadiusHigh;
}

// Called every frame
void ASensorPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASensorPool::AssignSensor(ATrafficCar * car, bool proxy)
{
	if (proxy)
	{
		AProxySensor* newSensor = nullptr;
		if (HasReserveProxies())
		{
			newSensor = reservedProxies.Pop();
		}
		else
		{
			newSensor = Cast<AProxySensor>(GetWorld()->SpawnActor(AProxySensor::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator));
		}
		newSensor->AssignCar(car);
		assignedProxies.Add(newSensor);
	}
	else
	{
		ACarSensorArray* newSensor = nullptr;
		if (HasReserveSensors())
		{
			newSensor = reservedSensors.Pop();
		}
		else
		{
			newSensor = Cast<ACarSensorArray>(GetWorld()->SpawnActor(SensorArrayClass, &FVector::ZeroVector, &FRotator::ZeroRotator));
		}
		newSensor->AssignCar(car);
		assignedSensors.Add(newSensor);
	}
}

void ASensorPool::RevokeSensor(ATrafficCar * car)
{
	ACarSensorArray* sensorArray = car->SensorArray;
	if (!sensorArray)
		return;
	if (sensorArray->IsProxy())
	{
		AProxySensor* revokedProxy = (AProxySensor*)sensorArray;
		assignedProxies.RemoveSwap(revokedProxy);
		reservedProxies.Add(revokedProxy);
	}
	else
	{
		assignedSensors.RemoveSwap(sensorArray);
		reservedSensors.Add(sensorArray);
	}
	sensorArray->RemoveCar();
}

void ASensorPool::UpdateSensors()
{
	TArray<ACarSensorArray*> removedSensors;
	TArray<AProxySensor*> removedProxies;
	int numSensors = assignedSensors.Num(), numProxies = assignedProxies.Num();
	int l = FMath::Max(numSensors, numProxies) - 1;
	ACarSensorArray* currentSensor;
	AProxySensor* currentProxy;
	for (; l >= 0; l--)
	{
		currentSensor = numSensors > l ? assignedSensors[l] : nullptr;
		currentProxy = numProxies > l ? assignedProxies[l] : nullptr;
		FVector sensorLocation = currentSensor ? currentSensor->GetActorLocation() : FVector::ZeroVector;
		FVector proxyLocation = currentProxy ? currentProxy->GetActorLocation() : FVector::ZeroVector;
		for (FVector critLocation : criticalLocations)
		{
			if (currentSensor && FVector::Dist(sensorLocation, critLocation) > swapRadiusHighSq)
			{
				removedSensors.Add(currentSensor);
			}
			if (currentProxy && FVector::Dist(proxyLocation, critLocation) < swapRadiusLowSq)
			{
				removedProxies.Add(currentProxy);
			}
		}
	}
	for (ACarSensorArray* sensor : removedSensors)
	{
		ATrafficCar* car = sensor->MyCar;
		assignedSensors.RemoveSwap(sensor);
		reservedSensors.Add(sensor);
		sensor->RemoveCar();
		AssignSensor(car);
	}
	for (AProxySensor* proxy : removedProxies)
	{
		ATrafficCar* car = proxy->MyCar;
		assignedProxies.RemoveSwap(proxy);
		reservedProxies.Add(proxy);
		proxy->RemoveCar();
		AssignSensor(car, false);
	}
}

bool ASensorPool::HasReserveProxies()
{
	return reservedProxies.Num() > 0;
}

bool ASensorPool::HasReserveSensors()
{
	return reservedSensors.Num() > 0;
}

