#include "TrafficVisualizer.h"

IMPLEMENT_HIT_PROXY(HTrafficVisProxy, HComponentVisProxy);
IMPLEMENT_HIT_PROXY(HTrafficProbeVisProxy, HTrafficVisProxy);

void FTrafficVisualizer::DrawVisualization(const UActorComponent * Component, const FSceneView * View, FPrimitiveDrawInterface * PDI)
{
	const UTrafficProbeComponent* trafficProbeComponent = Cast<const UTrafficProbeComponent>(Component);
	if (trafficProbeComponent)
	{
		const FTrafficData* data = trafficProbeComponent->GetTrafficData();
		if(trafficProbeComponent->)
	}
}
