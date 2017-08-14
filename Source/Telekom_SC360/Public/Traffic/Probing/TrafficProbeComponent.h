// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"

#include "TrafficProbeComponent.generated.h"

/**
 * 
 */
UCLASS()
class TELEKOM_SC360_API UTrafficProbeComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Traffic|Probing")
		bool bShowSpeedCurveDebug
	
	
};
