// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/SplineComponent.h"
#include "Classes/Curves/CurveFloat.h"

#include "Telekom_SC360.h"
#include "TrafficCar.h"

#include "TrafficRoad.generated.h"

class ATrafficCar;
class UCurveFloat;
class USplineComponent;

UCLASS()
class TELEKOM_SC360_API ATrafficRoad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrafficRoad();

	/**The speed limit of the path (in uu/s)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float baseSpeed;
	/**The margin with which the speed of an individual car can differ from the base speed (in uu/s)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float speedVariance;
	/**The weight of the path (when deciding on the path to take at a crossroads cars prefer the path with higher weight)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float roadWeight;
	/**The path originating from this path*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<ATrafficRoad*> childRoads;
	/**The speed of cars at a given distance, relative to the speed of the road*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UCurveFloat* speedCurve;

	/**The spline representing the road*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USplineComponent* spline;
	/**The current cars taking the path*/
	UPROPERTY(BlueprintReadWrite)
		TArray<ATrafficCar*> currentCars;
	UPROPERTY(BlueprintReadOnly)
		TArray<ATrafficCar*> removeList;
	/**The length of the road (in uu)*/
	UPROPERTY(BlueprintReadWrite)
		float length;

	///**This Event property for the traffic tick callbacks*/
	//UPROPERTY(BlueprintAssignable)
	//	FTrafficTick_Road TrafficTick;

	/**Adds a car to the path*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Road")
		virtual void AddCar(ATrafficCar* newCar);
	/**Called when a car has reached the end of the path, and the path has child paths*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Road")
		virtual void CarFinished(ATrafficCar* car, ATrafficRoad* forcedRoad);


	/**Gets the shortest route to the given path (using A*)*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Road")
		TArray<ATrafficRoad*> GetPathTo(ATrafficRoad* goal);
	/**Gets the distance between the middle point of this road, and the middle point of another one*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Road")
		float GetMinDistanceToRoad(ATrafficRoad* other);
	/**Gets the Distance between the middle point of this road, and an actor*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Road")
		float GetMinDistanceToActor(AActor* other);
	/**Gets the Distance between the middle point of this road, and a specified world location*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Road", meta = (DisplayName = "GetMinDistanceTo"))
		float GetMinDistanceToLocation(FVector pos);
	/**Returns the car with the specified uniqueID, or nullptr if there's no such car on the road*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Road")
		ATrafficCar* GetCarByID(int carID);
	/**Returns if the path has no child paths*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Road")
		virtual bool IsLeaf();
	/**Returns the transform at a given distance along the path (can be overriden to extend functionality)*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Road")
		virtual FTransform GetTransformAtTime(float time, ESplineCoordinateSpace::Type splineCoordinateSpaceType, int carID);
	/**Returns the desired speed of the car at the defined time on the road*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Road")
		virtual float GetDesiredSpeed(float time, int carID);
	/**Returns true if the car can leave the road*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Road")
		virtual bool CanLeaveRoad(float time, int carID);
		

	/**Called every traffic frame, propagates the tick to the cars taking this path*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Road")
		virtual void FTrafficTick(float DeltaT);
	/***/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Road")
		virtual void DetachCar(ATrafficCar* car);

	/***/
	UFUNCTION(BlueprintNativeEvent, Category = "Traffic|Road")
		void Initialize();
	/**Called when a car has reached the end of the path, and the path has child paths*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Road")
		void OnCarFinished(ATrafficCar* car, ATrafficRoad* forcedRoad);
	/**Called before every traffic frame*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Road")
		void BeforeTrafficTick(float DeltaT);
	/**Called after every traffic frame*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Road")
		void AfterTrafficTick(float DeltaT);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Initialize_Internal();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
