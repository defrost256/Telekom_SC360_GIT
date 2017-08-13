// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "DebugRenderSceneProxy.h"
#include "Classes/Engine/Canvas.h"
#include "GameFramework/Actor.h"

#include "TrafficProbeRenderComponent.generated.h"

class TELEKOM_SC360_API FTrafficDebugRenderSceneProxy : public FDebugRenderSceneProxy
{
	friend class FTrafficRenderingDebugDrawDelegateHelper;

public:	
	explicit FTrafficDebugRenderSceneProxy(
		const UPrimitiveComponent& InComponent, 
		const FString& ViewFlagName = TEXT("DebugAI"), 
		const TArray<FDebugLine>& Lines = TArray<FDebugLine>(), 
		const TArray<FText3d>& Texts = TArray<FText3d>()
	);

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

private:

	AActor* ActorOwner;

};

class FTrafficRenderingDebugDrawDelegateHelper : public FDebugDrawDelegateHelper
{
protected:
	TELEKOM_SC360_API virtual void DrawDebugLabels(UCanvas* Canvas, APlayerController*) override;

private:
	AActor* ActorOwner;
};

/**
 * 
 */
UCLASS()
class TELEKOM_SC360_API UTrafficProbeRenderComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
private:

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform &LocalToWorld) const override;
	virtual void CreateRenderState_Concurrent() override;
	virtual void DestroyRenderState_Concurrent() override;
	
	void ClearDebugStoredData();
	void StoreDebugData();

	FTrafficRenderingDebugDrawDelegateHelper TrafficDebugDrawDelegateHelper;

protected:

	TArray<FDebugRenderSceneProxy::FText3d> DebugTexts;
	TArray<FDebugRenderSceneProxy::FDebugLine> DebugLines;
	
};
