// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CarSensorArray.h"

#include "ProxySensor.generated.h"

/**
 * 
 */
UCLASS()
class TELEKOM_SC360_API AProxySensor : public ACarSensorArray
{
	GENERATED_BODY()
	
	// Sets default values for this actor's properties
	AProxySensor();

	//--------------------- ACarSensorArray implementation

	virtual float GetAvgSpeedOfOverlapCars() override;
	virtual void ChangeSensorDirection(ESensorDirection newDirection) override;
	virtual void GetOverlappingCars(TSet<AActor*>& actors, TSubclassOf<AActor> classFilter = nullptr) override;
	virtual FString GetCurrentSensorDirectionName() override;
	virtual bool IsOverlapping() override;
	virtual void AssignCar(ATrafficCar* car) override;
	virtual void RemoveCar() override;

	//--------------------- ACarSensorArray END
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
