// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/BoxComponent.h"

#include "CarSensorArray.h"
#include "TrafficHelper.h"

#include "CarCollisionSensor.generated.h"

class ATrafficCar;

UCLASS()
class TELEKOM_SC360_API ACarCollisionSensor : public ACarSensorArray
{
	GENERATED_BODY()
	
public:	

	/**The area where the car can sense other cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* FrontSensor;
	/**The area where the car can sense other cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* FrontRightSensor;
	/**The area where the car can sense other cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* FrontLeftSensor;
	/**The area where the car can sense other cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* LeftSensor;
	/**The area where the car can sense other cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* RightSensor;
	/**The area where the car can sense other cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* RearSensor;
	/**The area where the car can sense other cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* EmergencySensor;

	/**The number of cars currently in the sensing area of this car*/
	UPROPERTY(BlueprintReadWrite)
		int emergencyOverlap;
	/**The number of cars currently in the sensing area of this car*/
	UPROPERTY(BlueprintReadWrite)
		int overlapCount;
	UPROPERTY(BlueprintReadWrite)
		ESensorDirection activeDirection;

public:

	// Sets default values for this actor's properties
	ACarCollisionSensor();

	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void OnSensorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void OnSensorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void OnEmergencySensorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void OnEmergencySensorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Car")
		UBoxComponent* GetSensorByDirection(ESensorDirection dir);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		UBoxComponent* GetCurrentSensor();

//--------------------- ACarSensorArray implementation

	virtual float GetAvgSpeedOfOverlapCars() override;
	virtual void ChangeSensorDirection(ESensorDirection newDirection) override;
	virtual FString GetCurrentSensorDirectionName() override;
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
