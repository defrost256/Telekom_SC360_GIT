// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TrafficDataCollectorInterface.generated.h"

USTRUCT(BlueprintType)
struct FTrafficData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FVector>	Location_DebugTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<float> Time_DebugTime;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		int DataCount_DebugTime;

public:

	FTrafficData();

	void AddDebugTimeData(FVector loc, float time);
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTrafficDataCollectorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TELEKOM_SC360_API ITrafficDataCollectorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

		virtual FTrafficData GetTrafficData();
};
