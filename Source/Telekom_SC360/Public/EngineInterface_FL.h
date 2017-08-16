// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"

#include "EngineInterface_FL.generated.h"

/**
 * 
 */
UCLASS()
class TELEKOM_SC360_API UEngineInterface_FL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "EI|Debug", meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
		static void DrawDebugArc(UObject* WorldContextObject, FVector Center, float Radius, float FillRatio, int32 NumSegments, FLinearColor LineColor, float LifeTime, float Thickness, FRotator Rotation, bool bDrawAxis);
	static void DrawDebugArc_Internal(UWorld* WorldContextObject, FVector Center, float Radius, float FillRatio, int32 NumSegments, FLinearColor LineColor, float LifeTime, float Thickness, FRotator Rotation, bool bDrawAxis);

	static ULineBatchComponent* GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground)
	{
		return (InWorld ? (bDepthIsForeground ? InWorld->ForegroundLineBatcher : ((bPersistentLines || (LifeTime > 0.f)) ? InWorld->PersistentLineBatcher : InWorld->LineBatcher)) : NULL);
	}
};
