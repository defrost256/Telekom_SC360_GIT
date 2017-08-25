// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructorHelpers.h"
#include "Classes/Components/BoxComponent.h"
#include "Classes/Components/BillboardComponent.h"
#include "Classes/Engine/Texture2D.h"
#include "Classes/Engine/World.h"

#include "TrafficHelper.h"
#include "TrafficRoad.h"
#include "SensorPool.h"

#include "TrafficEmitter.generated.h"

class ATrafficRoad;
class ATrafficCar;
class UBoxComponent;

UCLASS()
class TELEKOM_SC360_API ATrafficEmitter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrafficEmitter();

	/**The prefabs this emitter can spawn*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<ATrafficCar>> carPrefabs;
	/**The time in seconds between a car leaving a spawn area, and another spawning*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float spawnFrequency;
	/**The time in seconds by which we can differ from the base spawn frequency*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float spawnVariance;
	/**The root path to which the newly spawned cars are added to*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ATrafficRoad* rootRoad;
	/**The paths the cars from here need to include (if empty the cars will decide their route by path weight)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<ATrafficRoad*> includeRoads;
	/**The chance of a car being forced to include the paths specified (ignored if include paths is empty)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float forcePathRatio;

	UPROPERTY(BlueprintReadWrite)
		UBillboardComponent* root;
	/**The area which the emitter checks to see if there is no car in the way of spawning*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* startArea;
	/**The list of cars currently traversing the graph*/
	UPROPERTY(BlueprintReadWrite)
		TArray<ATrafficCar*> carsActive;
	/**The cars held by the car pool*/
	UPROPERTY(BlueprintReadWrite)
		TArray<ATrafficCar*> carsReserve;
	///**Callback event for traffick frames*/
	//UPROPERTY(BlueprintAssignable)
	//	FTrafficTick_Emitter TrafficTick;
	/**The remaining time before another car can be spawned*/
	UPROPERTY(BlueprintReadWrite)
		float timeToNextSpawn;
	/**True if there is a car obstructing the spawn area*/
	UPROPERTY(BlueprintReadWrite)
		bool hasCarInSpawn;
	/**The full route the cars following the include paths take*/
	UPROPERTY(BlueprintReadOnly)
		TArray<ATrafficRoad*> forcedPath;
	UPROPERTY(BlueprintReadOnly)
		ASensorPool* sensorPool;

	/**Spawns a car from the emitter (if there is any car in the pool we respawn it)*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Emitter")
		void SpawnCar();
	/**Returns if there is any reserve cars in the car pool*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Emitter")
		bool HasReserves();
	/**Returns the start of the root path of the emitter*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Traffic|Emitter")
		FTransform GetStartTransform();
	/**Callback function for when a car has finished its route, and has nowhere to go*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Emitter")
		void CarFinished(ATrafficCar* car);

	/**Callback function called every traffic frame*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Emitter")
		void FTrafficTick(float DeltaT);
	/**Called when a car enters the spawn area*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Emitter")
		void OnStartAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	/**Called when a car leaves the spawn area*/
	UFUNCTION(BlueprintCallable, Category = "Traffic|Emitter")
		void OnStartAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Emitter")
		int KillPercentOfCars(float killPercent);

	/**Called before every traffic frame*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Car")
		void BeforeTrafficTick(float DeltaT);
	/**Called after every traffic frame*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Car")
		void AfterTrafficTick(float DeltaT);
	UFUNCTION(BlueprintNativeEvent, Category = "Traffic|Emitter")
		void Initialize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
