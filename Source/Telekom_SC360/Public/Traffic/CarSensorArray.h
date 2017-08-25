// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CarSensorArray.generated.h"

class ATrafficCar;

UENUM(BlueprintType)
enum class ESensorDirection : uint8
{
	Front,
	FrontRight,
	FrontLeft,
	Right,
	Left,
	Rear
};

UCLASS()
class TELEKOM_SC360_API ACarSensorArray : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadWrite)
		ATrafficCar* MyCar = nullptr;



	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensor")
		virtual float GetAvgSpeedOfOverlapCars();
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensor")
		virtual void ChangeSensorDirection(ESensorDirection newDirection);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensor")
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensor")
		virtual FString GetCurrentSensorDirectionName();
		virtual void GetOverlappingCars(TSet<AActor*>& outActors, TSubclassOf<AActor> classFilter = nullptr, bool emergency = true);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensor")
		virtual void AssignCar(ATrafficCar* car);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensor")
		virtual void RemoveCar();

	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensor")
		virtual bool IsOverlapping();
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensor")
		virtual bool IsEmergency();
	UFUNCTION(BlueprintCallable, Category = "Traffic|Sensor")
		virtual bool IsProxy();

	static FString GetSensorDirectionName(ESensorDirection dir);

	// Sets default values for this actor's properties
	ACarSensorArray();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
