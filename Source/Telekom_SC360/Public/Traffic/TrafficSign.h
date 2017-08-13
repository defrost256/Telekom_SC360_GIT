// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"



#include "TrafficSign.generated.h"

UCLASS()
class TELEKOM_SC360_API ATrafficSign : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrafficSign();

	UPROPERTY(BlueprintReadWrite)
		UPrimitiveComponent* semaphore;

	UFUNCTION(BlueprintCallable, Category = "Traffic")
		void StopTraffic();
	UFUNCTION(BlueprintCallable, Category = "Traffic")
		void StartTraffic();

	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Event")
		void OnStopTraffic();
	UFUNCTION(BlueprintImplementableEvent, Category = "Traffic|Event")
		void OnStartTraffic();

	UFUNCTION(BlueprintNativeEvent, Category = "Traffic|Event")
		void Initialize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};

UENUM(BlueprintType)
enum class ETrafficSignGroupPhase : uint8
{
	None,
	PhaseOne,
	PhaseTwo
};

USTRUCT(BlueprintType)
struct FTrafficSignGroupParam
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<ATrafficSign*> phaseOneSign;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<ATrafficSign*> phaseTwoSign;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DelayBetweenSwitch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DelayBeforeSwitch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ETrafficSignGroupPhase startingPhase;
};