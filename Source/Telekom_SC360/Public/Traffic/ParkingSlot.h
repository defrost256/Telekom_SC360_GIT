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
	
	/**The billboard component for easy handling*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBillboardComponent* billboard;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UCurveFloat* exitSpeedCurve;
	/**Sets if the spline should fit the ParkingPassthrough location and tangent or not (at BeginPlay?)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAutoSpline;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float parkTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float parkTimeVariance;
	/**The distancealongetime of the  the car entered the */
	UPROPERTY(BlueprintReadWrite)
		float carStartTime;
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

	virtual void AddCar(ATrafficCar* newCar) override;
	virtual bool IsLeaf() override;
	virtual FTransform GetTransformAtTime(float time, ESplineCoordinateSpace::Type splineCoordinateSpaceType, int carID) override;
	virtual float GetDesiredSpeed(float time, int carID) override;
	virtual bool CanLeaveRoad(float time, int carID) override;
	
	virtual void FTrafficTick(float DeltaT);
	virtual void DetachCar(ATrafficCar* car);

	void Initialize_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
