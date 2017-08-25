// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CarSensorArray.h"
#include "ProxySensor.h"
#include "TrafficCar.h"

#include "SensorPool.generated.h"

UCLASS()
class TELEKOM_SC360_API ASensorPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASensorPool();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<ACarSensorArray> SensorArrayClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FVector> criticalLocations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float swapRadiusLow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float swapRadiusHigh;


	UPROPERTY(BlueprintReadOnly)
		TArray<ACarSensorArray*> assignedSensors;
	UPROPERTY(BlueprintReadOnly)
		TArray<ACarSensorArray*> reservedSensors;
	UPROPERTY(BlueprintReadOnly)
		TArray<AProxySensor*> assignedProxies;
	UPROPERTY(BlueprintReadOnly)
		TArray<AProxySensor*> reservedProxies;
	UPROPERTY(BlueprintReadOnly)
		float swapRadiusLowSq;
	UPROPERTY(BlueprintReadOnly)
		float swapRadiusHighSq;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensors")
		void AssignSensor(ATrafficCar* car, bool proxy = true);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensors")
		void RevokeSensor(ATrafficCar* car);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensors")
		void UpdateSensors();
	
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensors")
		bool HasReserveProxies();
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensors")
		bool HasReserveSensors();
};
