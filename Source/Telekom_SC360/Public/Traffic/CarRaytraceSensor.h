// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Traffic/CarSensorArray.h"
#include "TrafficHelper.h"

#include "CarRaytraceSensor.generated.h"

class ATrafficCar;

/**
 * 
 */
UCLASS()
class TELEKOM_SC360_API ACarRaytraceSensor : public ACarSensorArray
{
	GENERATED_BODY()
	
public:

	ACarRaytraceSensor();
	
	/**The opening angle in which the car can detect other cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float viewAngle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int rayCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float maxRadius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float emergencyRadius;

	UPROPERTY(BlueprintReadWrite)
		TSet<ATrafficCar*> overlappedCars;
	UPROPERTY(BlueprintReadWrite)
		TSet<ATrafficCar*> emergencyCars;
	UPROPERTY(BlueprintReadWrite)
		TSet<AActor*> otherOverlaps;
	UPROPERTY(BlueprintReadWrite)
		float halfAngle;
	UPROPERTY(BlueprintReadWrite)
		FRotator deltaRotation;

	//--------------------- ACarSensorArray implementation

	virtual float GetAvgSpeedOfOverlapCars() override;
	virtual void ChangeSensorDirection(float deltaYaw) override;
	virtual void GetOverlappingCars(TSet<AActor*>& outActors, TSubclassOf<AActor> classFilter = nullptr, bool emergency = true) override;
	virtual bool IsOverlapping() override;
	virtual bool IsEmergency() override;
	virtual void AssignCar(ATrafficCar* car) override;
	virtual void RemoveCar() override;
	//virtual void IsProxy() override;

	//--------------------- ACarSensorArray END

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
