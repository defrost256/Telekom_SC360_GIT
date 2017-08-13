// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficProbe.h"


FVector4 ATrafficProbe::FindLocationAndDistanceAlongSplineNearLocation(USplineComponent* spline, FVector loc, float precision)
{
	float l = spline->GetSplineLength();
	float inc = l / 4;
	float d = l / 2;
	float targetKey = spline->FindInputKeyClosestToWorldLocation(loc);
	while (FMath::Abs(inc) > precision)
	{
		float currentKey = spline->GetInputKeyAtDistanceAlongSpline(d);
		if (currentKey < targetKey)
		{
			d += inc;
		}
		else if(currentKey > targetKey)
		{
			d -= inc;
		}
		inc /= 2;
	}
	return FVector4(spline->GetLocationAtSplineInputKey(targetKey, ESplineCoordinateSpace::World), d);
}

// Sets default values
ATrafficProbe::ATrafficProbe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ATrafficProbe::ATrafficProbe(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer),
	bDebugRoadTime(false),
	bAutoTargetRoad(false),
	targetRoad(nullptr)
{
	//Base code here set the collision profile to NoCollision (neccessary if pawn?)

#if WITH_EDITORONLY_DATA
	RenderComp = CreateEditorOnlyDefaultSubobject<UTrafficProbeRenderComponent>(TEXT("TrafficRender"));
	if (RenderComp)
	{
		RenderComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//Base code here initializes some other components
	//TODO Create a billboard for it
#endif // WITH_EDITORONLY_DATA

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//Base code here set the character movement to MOVE_None

#if WITH_EDITOR
	//if it's a final class
	if (HasAnyFlags(RF_ClassDefaultObject) && GetClass() == StaticClass())
	{
		USelection::SelectObjectEvent.AddStatic(&ATrafficProbe::OnEditorSelectionChanged);
	}
#endif // WITH_EDITOR


}

// Called when the game starts or when spawned
void ATrafficProbe::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrafficProbe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO Run Traffic Queries
}

void ATrafficProbe::PostLoad()
{
	Super::PostLoad();

	//TODO Setup billboard
	/*REF
	UBillboardComponent* SpriteComponent = FindComponentByClass<UBillboardComponent>();
	if (SpriteComponent != NULL)
	{
		SpriteComponent->bHiddenInGame = !bShouldBeVisibleInGame;
	}*/

	UWorld* World = GetWorld();
	PrimaryActorTick.bCanEverTick = World && (!World->IsGameWorld() || bTickDuringGame);
}
#if WITH_EDITOR
void ATrafficProbe::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	static const FName NAME_DebugRoadTime = GET_MEMBER_NAME_CHECKED(ATrafficProbe, bDebugRoadTime);
	static const FName NAME_AutoTargetRoad = GET_MEMBER_NAME_CHECKED(ATrafficProbe, bAutoTargetRoad);
	static const FName NAME_TargetRoad = GET_MEMBER_NAME_CHECKED(ATrafficProbe, targetRoad);
	static const FName NAME_VisibleInGame = GET_MEMBER_NAME_CHECKED(ATrafficProbe, bVisibleInGame);

	if (PropertyChangedEvent.Property)
	{
		const FName PropName = PropertyChangedEvent.MemberProperty->GetFName();
		if (PropName == NAME_DebugRoadTime)
		{
			if (bDebugRoadTime)
			{
				RunQueries();
			}
			UpdateDrawing();
		}
		else if (PropName == NAME_AutoTargetRoad && bDebugRoadTime)
		{
			RunQueries();
			UpdateDrawing();
		}
		else if (PropName == NAME_TargetRoad && bDebugRoadTime && bAutoTargetRoad)
		{
			RunQueries();
			UpdateDrawing();
		}
		else if (PropName == NAME_VisibleInGame)
		{
			UpdateDrawing(); //TODO Might not be neccessary (base class just does billboard stuff)
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ATrafficProbe::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished || !bUpdateOnlyOnFinishedMove)
	{
		RunQueries();
	}
}
#endif // WITH_EDITOR

FTrafficData ATrafficProbe::GetTrafficData()
{
	// TODO: insert return statement here
	return FTrafficData();
	
}

void ATrafficProbe::RunQueries()
{
	FTrafficData newData;
	UWorld* World = GetWorld();
	ATrafficRoad* nearestRoad = GetNearestRoad();
	FVector Me = GetActorLocation();
	if (bDebugRoadTime)
	{
		TArray<UActorComponent*> splineComps;
		if (bAutoTargetRoad)
		{
			if(nearestRoad)
				splineComps = nearestRoad->GetComponentsByClass(USplineComponent::StaticClass());
		}
		else
		{
			if(targetRoad)
				splineComps = targetRoad->GetComponentsByClass(USplineComponent::StaticClass());
		}
		for (UActorComponent* splineComp : splineComps)
		{
			USplineComponent* spline = Cast<USplineComponent>(splineComp);
			FVector4 locAndDist = FindLocationAndDistanceAlongSplineNearLocation(spline, Me);
			newData.AddDebugTimeData(locAndDist, locAndDist.W);
		}
	}

	currentData = newData;
}

void ATrafficProbe::UpdateDrawing()
{
#if WITH_EDITORONLY_DATA
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	//TODO Mark sprite component render state dirty
	/* REF
	UBillboardComponent* SpriteComponent = FindComponentByClass<UBillboardComponent>();
	if (SpriteComponent != NULL)
	{
		SpriteComponent->MarkRenderStateDirty();
	}	
	*/

	if (RenderComp && RenderComp->bVisible)
	{
		RenderComp->MarkRenderStateDirty();
	}

#if WITH_EDITOR
	if (GEditor)
	{
		GEditor->RedrawLevelEditingViewports();
	}
#endif // WITH_EDITOR

#endif // WITH_EDITORONLY_DATA

}

ATrafficRoad * ATrafficProbe::GetNearestRoad()
{
	TArray<AActor*> roadActors;
	UGameplayStatics::GetAllActorsOfClass(this, ATrafficRoad::StaticClass(), roadActors);
	FVector Me = GetActorLocation();
	if (roadActors.Num() > 0)
	{
		ATrafficRoad* nearestRoad = nullptr;
		float smallestDistance = INFINITY;
		for (AActor* roadActor : roadActors)
		{
			ATrafficRoad* road = Cast<ATrafficRoad>(roadActor);
			float d = road->GetMinDistanceToLocation(Me);
			if (d < smallestDistance)
			{
				smallestDistance = d;
				nearestRoad = road;
			}
		}
		return nearestRoad;
	}
	return nullptr;
}

void ATrafficProbe::OnEditorSelectionChanged(UObject * NewSelection)
{
}
