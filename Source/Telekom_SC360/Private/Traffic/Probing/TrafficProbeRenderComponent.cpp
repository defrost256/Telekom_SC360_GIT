// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficProbeRenderComponent.h"

void FTrafficRenderingDebugDrawDelegateHelper::DrawDebugLabels(UCanvas* Canvas, APlayerController* PC)
{
	//TODO Own test for draw or not again?

	// little hacky test but it's the only way to remove text rendering from bad worlds, when using UDebugDrawService for it
	if (Canvas && Canvas->SceneView && Canvas->SceneView->Family && Canvas->SceneView->Family->Scene && Canvas->SceneView->Family->Scene->GetWorld() != ActorOwner->GetWorld())
	{
		return;
	}

	FDebugDrawDelegateHelper::DrawDebugLabels(Canvas, PC);

}

FBoxSphereBounds UTrafficProbeRenderComponent::CalcBounds(const FTransform & LocalToWorld) const
{
	return FBoxSphereBounds(FBox(FVector(-HALF_WORLD_MAX), FVector(HALF_WORLD_MAX)));
}

FPrimitiveSceneProxy* UTrafficProbeRenderComponent::CreateSceneProxy()
{
	FTrafficDebugRenderSceneProxy* proxy = new FTrafficDebugRenderSceneProxy(*this, TEXT("TrafficDebug"), DebugLines, DebugTexts);
	if (proxy)
	{
		TrafficDebugDrawDelegateHelper.InitDelegateHelper(proxy);
		TrafficDebugDrawDelegateHelper.ReregisterDebugDrawDelgate();
	}
	return proxy;
}

void UTrafficProbeRenderComponent::CreateRenderState_Concurrent()
{
	Super::CreateRenderState_Concurrent();

	TrafficDebugDrawDelegateHelper.RegisterDebugDrawDelgate();
}

void UTrafficProbeRenderComponent::DestroyRenderState_Concurrent()
{
	TrafficDebugDrawDelegateHelper.UnregisterDebugDrawDelgate();

	Super::DestroyRenderState_Concurrent();
}

void UTrafficProbeRenderComponent::ClearDebugStoredData()
{
	DebugLines.Reset();
	DebugTexts.Reset();
	MarkRenderStateDirty();
}

void UTrafficProbeRenderComponent::StoreDebugData()
{
	//TODO IMPLEMENT
}

FTrafficDebugRenderSceneProxy::FTrafficDebugRenderSceneProxy(const UPrimitiveComponent & InComponent, const FString & InViewFlagName, const TArray<FDebugRenderSceneProxy::FDebugLine>& InLines, const TArray<FDebugRenderSceneProxy::FText3d>& InTexts)
	:FDebugRenderSceneProxy(&InComponent)
{
	DrawType = WireMesh;
	TextWithoutShadowDistance = 1500;
	ViewFlagIndex = uint32(FEngineShowFlags::FindIndexByName(*InViewFlagName));
	ViewFlagName = InViewFlagName;
	bWantsSelectionOutline = false;

	Lines = InLines;
	Texts = InTexts;
	
	const UTrafficProbeRenderComponent* renderComp = Cast<const UTrafficProbeRenderComponent>(&InComponent);
	
	ActorOwner = InComponent.GetOwner();
	DataSource = Cast<const ITrafficDataCollectorInterface>(DataSource);
	if (!DataSource)
	{
		DataSource = Cast<const ITrafficDataCollectorInterface>(&InComponent);
	}
	if (Lines.Num() == 0 && Texts.Num() == 0 && DataSource)
	{
		CollectData();
	}
}

FPrimitiveViewRelevance FTrafficDebugRenderSceneProxy::GetViewRelevance(const FSceneView * View) const
{
	FPrimitiveViewRelevance ret;
	ret.bDrawRelevance = View->Family->EngineShowFlags.GetSingleFlag(ViewFlagIndex) && IsShown(View);
	ret.bDynamicRelevance = true;
	ret.bSeparateTranslucencyRelevance = ret.bNormalTranslucencyRelevance = IsShown(View);
	return ret;
}

void FTrafficDebugRenderSceneProxy::CollectData()
{
}
