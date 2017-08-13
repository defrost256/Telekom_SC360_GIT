// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Telekom_SC360.h"

#include "TrafficSign.h"
#include "TrafficSignGroup.h"

#include "TrafficIntersection.generated.h"

class ATrafficSign;
class UTrafficSignGroup;
struct FTrafficSignGroupParam;
enum class ETrafficSignGroupPhase : uint8;

UCLASS()
class TELEKOM_SC360_API ATrafficIntersection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrafficIntersection();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FTrafficSignGroupParam> signGroupParams;
	UPROPERTY(BlueprintReadWrite)
		TArray<UTrafficSignGroup*> signGroups;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
