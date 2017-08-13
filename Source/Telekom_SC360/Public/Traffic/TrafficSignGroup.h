// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "TrafficSign.h"

#include "TrafficSignGroup.generated.h"

/**
 * 
 */
UCLASS()
class TELEKOM_SC360_API UTrafficSignGroup : public UObject
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
	UPROPERTY(BlueprintReadWrite)
		float timer;
	UPROPERTY(BlueprintReadWrite)
		ETrafficSignGroupPhase currentPhase;
	UPROPERTY(BlueprintReadWrite)
		ETrafficSignGroupPhase nextPhase;
public:
	void Initialize(FTrafficSignGroupParam params);
};
