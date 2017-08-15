// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficRoad.h"

// Sets default values
ATrafficRoad::ATrafficRoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATrafficRoad::BeginPlay()
{
	Super::BeginPlay();
	
}

float ATrafficRoad::GetMinDistanceToRoad(ATrafficRoad * other)
{
	return FVector::Dist(spline->GetWorldLocationAtTime(0.5), other->spline->GetWorldLocationAtTime(0.5));
}

float ATrafficRoad::GetMinDistanceToActor(AActor * other)
{
	return FVector::Dist(spline->GetWorldLocationAtTime(0.5), other->GetActorLocation());
}

float ATrafficRoad::GetMinDistanceToLocation(FVector pos)
{
	return FVector::Dist(spline->GetWorldDirectionAtTime(0.5), pos);
}

void ATrafficRoad::Initialize_Internal()
{
}

// Called every frame
void ATrafficRoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrafficRoad::AddCar(ATrafficCar * newCar)
{
	currentCars.Add(newCar);
	newCar->PutOnRoad(this, baseSpeed + FMath::RandRange(-speedVariance, speedVariance));
	UE_LOG(TrafficLog, Log, TEXT("ADD %s to %s"), *newCar->GetName(), *GetName());
}

void ATrafficRoad::CarFinished(ATrafficCar * car, ATrafficRoad * forcedRoad)
{
	OnCarFinished(car, forcedRoad);
	removeList.Add(car);
	if (childRoads.Num() == 0)
		return;
	ATrafficRoad* selectedRoad = nullptr;
	if (forcedRoad == nullptr)
	{
		float rand = FMath::FRandRange(0, 1);
		for (ATrafficRoad* road : childRoads) {
			if (road->roadWeight > rand)
			{
				selectedRoad = road;
				break;
			}
		}
	}
	else
		selectedRoad = forcedRoad;
	if (selectedRoad == nullptr)
		selectedRoad = childRoads[childRoads.Num() - 1];
	selectedRoad->AddCar(car);
	UE_LOG(TrafficLog, Log, TEXT("FINISHED Car %s (selected road: %s)"), *car->GetName(), *selectedRoad->GetName());

}

TArray<ATrafficRoad*> ATrafficRoad::GetPathTo(ATrafficRoad * goal)
{
	FVector goalPoint = goal->spline->GetWorldLocationAtTime(0.5);
	TSet<ATrafficRoad*> closedSet, openSet;
	TMap<ATrafficRoad*, ATrafficRoad*> cameFromMap;
	TMap<ATrafficRoad*, float> gScore, fScore;
	ATrafficRoad* currentRoad;

	openSet.Add(this);
	gScore.Add(this, 0);
	fScore.Add(this, GetMinDistanceToRoad(goal));
	while (openSet.Num() > 0)
	{
		currentRoad = nullptr;
		for (ATrafficRoad* r : openSet)
		{
			if (currentRoad == nullptr || fScore[r] < fScore[currentRoad])
				currentRoad = r;
		}
		if (currentRoad == goal)
		{
			TArray<ATrafficRoad*> ret;
			ret.Add(currentRoad);
			while (cameFromMap.Contains(currentRoad))
			{
				currentRoad = cameFromMap[currentRoad];
				ret.Add(currentRoad);
			}
			return ret;
		}
		openSet.Remove(currentRoad);
		closedSet.Add(currentRoad);
		for (ATrafficRoad* r : currentRoad->childRoads)
		{
			if (!gScore.Contains(r))
			{
				gScore.Add(r, MAX_FLT);
				fScore.Add(r, MAX_FLT);
			}
			float currentGScore;
			if (!closedSet.Contains(r))
			{
				currentGScore = gScore[currentRoad] + GetMinDistanceToRoad(r);
				if (!openSet.Contains(r))
					openSet.Add(r);
				if (currentGScore < gScore[r])
				{
					cameFromMap.Add(r, currentRoad);
					gScore[r] = currentGScore;
					fScore[r] = currentGScore + r->GetMinDistanceToRoad(goal);
				}
			}
		}
	}
	return TArray<ATrafficRoad*>();
}

ATrafficCar * ATrafficRoad::GetCarByID(int carID)
{
	for (ATrafficCar* car : currentCars)
	{
		if (car->GetUniqueID() == carID)
			return car;
	}
	return nullptr;
}

bool ATrafficRoad::IsLeaf()
{
	return childRoads.Num() == 0;
}

FTransform ATrafficRoad::GetTransformAtTime(float time, ESplineCoordinateSpace::Type splineCoordinateSpaceType, int carID)
{
	return spline->GetTransformAtDistanceAlongSpline(time, splineCoordinateSpaceType);
}

float ATrafficRoad::GetDesiredSpeed(float time, int carID)
{
	return (speedCurve == nullptr ? 1 : speedCurve->GetFloatValue(time));
}

bool ATrafficRoad::CanLeaveRoad(float time, int carID)
{
	return time > length;
}

void ATrafficRoad::FTrafficTick(float DeltaT)
{
	BeforeTrafficTick(DeltaT);
	for (ATrafficCar* car : currentCars)
	{
		car->FTrafficTick(DeltaT);
	}
	for (ATrafficCar* removeCar : removeList)
	{
		currentCars.Remove(removeCar);
	}
	removeList.Empty();
	AfterTrafficTick(DeltaT);
}

void ATrafficRoad::DetachCar(ATrafficCar * car)
{
	currentCars.Remove(car);
	UE_LOG(TrafficLog, Log, TEXT("DETACH %s from %s"), *car->GetName(), *GetName());

}

void ATrafficRoad::Initialize_Implementation()
{
	length = spline->GetSplineLength();
}
