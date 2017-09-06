// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Public/Logging/MessageLog.h"
#include "Public/Logging/TokenizedMessage.h"
#include "Public/Misc/UObjectToken.h"
#include "Engine.h"

#include "EngineInterface_FL.generated.h"

UENUM(BlueprintType)
enum class EMessageLogSeverity : uint8
{
	CriticalError = 0,
	Error = 1,
	PerformanceWarning = 2,
	Warning = 3,
	Info = 4,	// Should be last
};

/**
 * 
 */
UCLASS()
class TELEKOM_SC360_API UEngineInterface_FL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
	static TSharedPtr<FMessageLog> lastLog;
	static TSharedPtr<FTokenizedMessage> currentMessage;
	static FName lastLogName;
public:
	UFUNCTION(BlueprintCallable, Category = "EI|Debug", meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
		static void DrawDebugArc(UObject* WorldContextObject, FVector Center, float Radius, float FillRatio, int32 NumSegments, FLinearColor LineColor, float LifeTime, float Thickness, FRotator Rotation, bool bDrawAxis);
	static void DrawDebugArc_Internal(UWorld* WorldContextObject, FVector Center, float Radius, float FillRatio, int32 NumSegments, FLinearColor LineColor, float LifeTime, float Thickness, FRotator Rotation, bool bDrawAxis);

	static ULineBatchComponent* GetDebugLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground)
	{
		return (InWorld ? (bDepthIsForeground ? InWorld->ForegroundLineBatcher : ((bPersistentLines || (LifeTime > 0.f)) ? InWorld->PersistentLineBatcher : InWorld->LineBatcher)) : NULL);
	}
	UFUNCTION(BlueprintCallable, Category = "EI|Debug|Messages", meta = (DevelopmentOnly))
		static void CreateMessageLog(const FName& name);
	UFUNCTION(BlueprintCallable, Category = "EI|Debug|Messages", meta = (DevelopmentOnly))
		static void ClearMessageLog();
	UFUNCTION(BlueprintCallable, Category = "EI|Debug|Messages", meta = (DevelopmentOnly))
		static void StartComposing(EMessageLogSeverity Severity, const FText& Message);
	UFUNCTION(BlueprintCallable, Category = "EI|Debug|Messages", meta = (DevelopmentOnly))
		static void FinishComposing();
	UFUNCTION(BlueprintCallable, Category = "EI|Debug|Messages", meta = (DevelopmentOnly))
		static void ShowLog(EMessageLogSeverity Severity, bool bShowIfEmpty);
	UFUNCTION(BlueprintCallable, Category = "EI|Debug|Messages", meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
		static void AddUObjectToken(UObject* WorldContextObject, UObject* object, const FText& label);
	UFUNCTION(BlueprintCallable, Category = "EI|Debug|Messages", meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
		static void AddTextToken(UObject* WorldContextObject, const FText& text);
};
