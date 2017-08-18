// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CarSensorArray.h"
#include "ProxySensor.h"

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

	UPROPERTY(BlueprintReadOnly)
		TArray<ACarSensorArray*> assignedCarSensors;
	UPROPERTY(BlueprintReadOnly)
		TArray<ACarSensorArray*> reservedCarSensors;
	UPROPERTY(BlueprintReadOnly)
		TArray<AProxySensor*> assignedProxies;
	UPROPERTY(BlueprintReadOnly)
		TArray<AProxySensor*> reservedProxies;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensors")
		void AssignSensor(ATrafficCar* car);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensors")
		void RevokeSensor(ACarSensorArray* sensor);
	
	
};
