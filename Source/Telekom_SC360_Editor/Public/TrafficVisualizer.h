#pragma once

#pragma once

#include "ComponentVisualizer.h"

#include "Traffic/Probing/TrafficProbeComponent.h"
#include "Traffic/Probing/TrafficDataCollectorInterface.h"

class FTrafficVisualizer : public FComponentVisualizer
{
public:
	FTrafficVisualizer();
	virtual ~FTrafficVisualizer();

	// Begin FComponentVisualizer interface
	virtual void OnRegister() override;
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual bool VisProxyHandleClick(FLevelEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	virtual void EndEditing() override;
	virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;
	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;
	virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual TSharedPtr<SWidget> GenerateContextMenu() const override;
	// End FComponentVisualizer interface

	/** Get the target component we are currently editing */
	UTrafficProbeComponent* GetEditedProbeComponent() const;

private:
	/**Index of target in selected component*/
	int32 CurrentlySelectedTarget;

	/**Output log commands*/
	TSharedPtr<FUICommandList> TargetingComponentVisualizerActions;
};

struct HTrafficVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	HTrafficVisProxy(const UActorComponent* InComponent) : HComponentVisProxy(InComponent, HPP_Wireframe) {}
};

struct HTrafficProbeVisProxy : public HTrafficVisProxy
{
	DECLARE_HIT_PROXY();

	HTrafficProbeVisProxy(const UActorComponent* InComponent) : HTrafficVisProxy(InComponent) {}
};