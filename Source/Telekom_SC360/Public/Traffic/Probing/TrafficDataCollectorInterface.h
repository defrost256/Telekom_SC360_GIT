// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TrafficDataCollectorInterface.generated.h"

UENUM(BlueprintType)
enum class ETrafficDataType : uint8
{
	None = 0,
	SpeedCurve = 1
};

USTRUCT(BlueprintType)
struct FTrafficData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FVector>	Location_SpeedCurve;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<float> Time_SpeedCurve;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<float> Speed_SpeedCurve;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FColor Color_SpeedCurve;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		int DataCount_DebugTime;

public:

	FTrafficData();

	void AddDebugTimeData(FVector loc, float time, float speed);
	void SetColorOf(ETrafficDataType type, FColor color);
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

		virtual const FTrafficData* GetTrafficData() const;
};
