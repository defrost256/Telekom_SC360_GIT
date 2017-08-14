// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/BillboardComponent.h"

#include "Traffic/TrafficCar.h"

#include "ParkingSlot.generated.h"

UENUM(BlueprintType)
enum class EParkingState : uint8
{
	None,
	Arriving,
	Parking,
	Leaving
};

UCLASS()
class TELEKOM_SC360_API AParkingSlot : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USplineComponent* entrySpline;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USplineComponent* exitSpline;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBillboardComponent* billboard;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAutoSpline;
	UPROPERTY(BlueprintReadWrite)
		ATrafficCar* currentCar;
	UPROPERTY(BlueprintReadWrite)
		float carStartTime;
	UPROPERTY(BlueprintReadWrite)
		float entryTime;
	UPROPERTY(BlueprintReadWrite)
		EParkingState state;

public:	

	// Sets default values for this actor's properties
	AParkingSlot();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|PSlot")
		bool CanCarStartParking();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|PSlot")
		bool IsCarCloseEnough();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|PSlot")
		bool HasCar();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|PSlot")
		bool CanCarExit();
	UFUNCTION(BlueprintCallable, Category = "Traffic|PSlot")
		bool ParkCar(float time);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|PSlot")
		FTransform GetTransformAtTime(float time, ESplineCoordinateSpace::Type coordinateSpace);
	UFUNCTION(BlueprintCallable, Category = "Traffic|PSlot")
		bool SetCar(ATrafficCar* car, float time);
	UFUNCTION(BlueprintCallable, Category = "Traffic|PSlot")
		ATrafficCar* RemoveCar();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|PSlot")
		float GetDesiredSpeed(float time);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
