// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TrafficEmitter.h"
#include "TrafficRoad.h"
#include "SensorPool.h"

#include "TrafficManager.generated.h"

class ATrafficEmitter;
class ATrafficRoad;

UCLASS()
class TELEKOM_SC360_API ATrafficManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrafficManager();

	/**The roads of the Traffic Graph*/
	UPROPERTY(BlueprintReadWrite)
		TArray<ATrafficRoad*> roads;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ASensorPool* sensorPool;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float sensorPoolUpdateRate;
	UPROPERTY(BlueprintReadWrite)
		float sensorPoolUpdateTime;
	/**The emitters in the scene, which emit cars*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<ATrafficEmitter*> emitters;

	UFUNCTION(BlueprintCallable, Category = "Traffic|Util")
		void KillPercentOfAllCars(float killPercent);
	UFUNCTION(BlueprintCallable, Category = "Traffic|Util")
		void ScaleAllEmitterRate(float scale);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
