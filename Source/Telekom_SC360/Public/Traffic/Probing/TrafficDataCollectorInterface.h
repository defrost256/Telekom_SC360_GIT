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
		FVector	Location_DebugTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Time_DebugTime;
public:

	FTrafficData();
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
