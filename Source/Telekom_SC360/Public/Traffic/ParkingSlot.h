// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/BillboardComponent.h"

#include "TrafficRoad.h"
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
class TELEKOM_SC360_API AParkingSlot : public ATrafficRoad
{
	GENERATED_BODY()
public:
	//For the entry spline we use the default spline of ATrafficRoad
	/**The spline the car uses to exit the slot*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USplineComponent* exitSpline;			// the exit spline's first point should be (0, 0, 0) relative to the Root, and the tangent should face outwardsfrom the ParkingSlot
	/**The billboard component for easy handling*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBillboardComponent* billboard;
	/**Sets if the spline should fit the ParkingPassthrough location and tangent or not (at BeginPlay?)*/
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
