// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/DrawDebugHelpers.h"

#include "TrafficRoad.h"
#include "TrafficEmitter.h"
#include "TrafficHelper.h"

#include "TrafficCar.generated.h"

class ATrafficRoad;
class ATrafficEmitter;

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
class TELEKOM_SC360_API ATrafficCar : public AActor
{
	GENERATED_BODY()
	
public:	

	/**The speed at which the car accelerates (uu/s^2)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float accelerationLerpSpeed;
	/**The speed at which the car decelerates when it detects an obstacle(uu/s^2)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float decelerationLerpSpeed;
	/**A multiplier applied to the road speed to determine the max speed of the car*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float speedMultiplier;
	/**A toggle for debug draws*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool debugDraw;

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

	/**The area sensed by other cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* sensedArea;

	/**The path the car is currently on*/
	UPROPERTY(BlueprintReadWrite)
		ATrafficRoad* road;
	UPROPERTY(BlueprintReadWrite)
		ESensorDirection activeDirection;
	/**The distance of the car along the current path*/
	UPROPERTY(BlueprintReadWrite)
		float time;
	/**The max speed at which this car can travel on the current path (in uu/s)*/
	UPROPERTY(BlueprintReadWrite)
		float maxSpeed;
	/**The current speed of the car (in uu/s)*/
	UPROPERTY(BlueprintReadWrite)
		float speed;
	/**The speed the car is trying to achieve (in uu/s) (this is affected by the speed limit, and speed curve of the road)*/
	UPROPERTY(BlueprintReadWrite)
		float targetSpeed;
	/**The number of cars currently in the sensing area of this car*/
	UPROPERTY(BlueprintReadWrite)
		int emergencyOverlap;
	/**The number of cars currently in the sensing area of this car*/
	UPROPERTY(BlueprintReadWrite)
		int overlapCount;
	UPROPERTY(BlueprintReadWrite)
		float slowdownMultiplier;
	/**True if the car's path is unobstructed*/
	UPROPERTY(BlueprintReadWrite)
		bool freePath;
	/**True if the car is paused by the PauseCar method*/
	UPROPERTY(BlueprintReadWrite)
		bool paused;
	/**The duration of the pause (negative if infinite)*/
	UPROPERTY(BlueprintReadWrite)
		float pauseLimit;
	/**The time the car spent paused since the last PauseCar call*/
	UPROPERTY(BlueprintReadWrite)
		float pauseTime;
	/**The list of paths the car should take (if empty the car chooses based on the weight of the path)*/
	UPROPERTY(BlueprintReadWrite)
		TArray<ATrafficRoad*> forcedPath;
	/**The emitter this car originates from*/
	UPROPERTY(BlueprintReadWrite)
		ATrafficEmitter* emitter;
	/**The index of the path the car currently is on from the forced paths*/
	UPROPERTY(BlueprintReadWrite)
		int forcedProgress;

	/***/
	UPROPERTY(BlueprintReadOnly)
		float forcedStopTime;
	UPROPERTY(BlueprintReadWrite)
		float forcedStopLimit = 1;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Car")
		int GetUniqueID_Int();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Car")
		bool IsStopped(float tolerance = 0.00001f);
	/**Despawns the car (this prevents the car from moving, or being sensed by other cars)*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void Despawn();
	/**(Re)spawns the car (this sets the car to the given transform, and enables it to receive ticks, and be sensed by other cars)*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void Respawn(FTransform spawnPoint);
	/**Removes the car from the road, and notifies the Emitter to despawn the car, and put it back in the InactiveQueue*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void Kill();
	/**Pauses the car, the Car won't tick, and thus it won't move for the specified duration or until UnpauseCar is called. (negative duration means infinite duration)*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void PauseCar(float duration = -1);
	/**Unpauses the car, allowing it to move and tick. */
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void UnpauseCar();
	/**Assigns the road to the car, resets time, and sets the baseSpeed*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void PutOnRoad(ATrafficRoad* newRoad, float roadSpeed, float _time = 0);
	/**Called every traffic frame, when the car is running (active)*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void FTrafficTick(float DeltaT);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void OnSensorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void OnSensorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void OnEmergencySensorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void OnEmergencySensorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		bool ResolveDeadlock();
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void Bump(float size = 0.0001f);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Car")
		UBoxComponent* GetSensorByDirection(ESensorDirection dir);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		void ChangeSensorDirection(ESensorDirection newDirection);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		UBoxComponent* GetCurrentSensor();
	UFUNCTION(BlueprintCallable, Category = "Traffic|Car")
		float GetAvgSpeedOfOverlapCars();

	/**Called before every traffic frame*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Car")
		void BeforeTrafficTick(float DeltaT);
	/**Called after every traffic frame*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Car")
		void AfterTrafficTick(float DeltaT);
	/**Called before a car is spawned*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Car")
		void OnRespawn(FTransform spawnPoint);

	// Sets default values for this actor's properties
	ATrafficCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetWorldTransformNoScale(FTransform transform);	
};
