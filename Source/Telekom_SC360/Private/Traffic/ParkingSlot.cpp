// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkingSlot.h"


bool AParkingSlot::CanCarStartParking(FVector position)
{
	float d = FVector::Dist(position, spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
	//UE_LOG(TrafficLog, Log, TEXT("Distance to parking spot"));
	return d < carGrabDistance;
}

bool AParkingSlot::IsCarCloseEnough()
{
	if (!HasCar())
		return false;
	return GetDistanceTo(currentCars[0]) < carGrabDistance;
}

bool AParkingSlot::HasCar()
{
	return currentCars[0] != nullptr;
}

void AParkingSlot::CarFinished(ATrafficCar * car, ATrafficRoad * forcedRoad)
{
	OnCarFinished(car, forcedRoad);
	currentCars[0] = nullptr;
	parent->AddCarFromSlot(car);
	state = EParkingState::None;
	//UE_LOG(TrafficLog, Log, TEXT("FINISHED Car %s (selected road: %s)"), *car->GetName(), *parent->GetName());
}

void AParkingSlot::AddCar(ATrafficCar * newCar)
{
	if (HasCar())
	{
		newCar->Kill();
		return;
	}
	carStartTime = newCar->time;
	state = EParkingState::Arriving;
	currentCars[0] = newCar;
	newCar->PutOnRoad(this, baseSpeed + FMath::RandRange(-speedVariance, speedVariance));
	//UE_LOG(TrafficLog, Log, TEXT("ADD %s to %s"), *newCar->GetName(), *GetName());
}

bool AParkingSlot::IsLeaf()
{
	return false;
}

FTransform AParkingSlot::GetTransformAtTime(float time, ESplineCoordinateSpace::Type splineCoordinateSpaceType, int carID)
{
	if (time > length)
	{
		return spline->GetTransformAtDistanceAlongSpline(2 * length - time, splineCoordinateSpaceType);
	}
	return spline->GetTransformAtDistanceAlongSpline(time, splineCoordinateSpaceType);
}

float AParkingSlot::GetDesiredSpeed(float time, int carID)
{
	if (time > length)
	{
		return (exitSpeedCurve ? exitSpeedCurve->GetFloatValue(2 * length - time) : 1);
	}
	return (speedCurve ? speedCurve->GetFloatValue(time) : 1);
}

bool AParkingSlot::CanLeaveRoad(float time, int carID)
{
	if (!HasCar() || state != EParkingState::Leaving)
		return false;
	return FVector::Dist(currentCars[0]->GetActorLocation(), spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World)) < carGrabDistance;
}

void AParkingSlot::FTrafficTick(float DeltaT)
{
	BeforeTrafficTick(DeltaT);
	if (HasCar())
	{
		if (state == EParkingState::Arriving && IsCarCloseEnough())
		{
			state = EParkingState::Parking;
			currentCars[0]->PauseCar(parkTime + FMath::RandRange(-parkTimeVariance, parkTimeVariance));
			state = EParkingState::Leaving;
		}
		else
		{
			if (currentCars[0]->IsStopped())
				currentCars[0]->Bump();
			currentCars[0]->FTrafficTick(DeltaT);
		}
	}
	AfterTrafficTick(DeltaT);
}

void AParkingSlot::DetachCar(ATrafficCar * car)
{
	if (!HasCar())
		return;
	//UE_LOG(TrafficLog, Log, TEXT("DETACH %s from %s"), *currentCars[0]->GetName(), *GetName());
	currentCars[0] = nullptr;
	state = EParkingState::None;
}

void AParkingSlot::Initialize_Implementation()
{
	Super::Initialize_Implementation();
	parent = Cast<AParkingPassthrough>(childRoads[0]);
}

// Sets default values
AParkingSlot::AParkingSlot()
{
	currentCars.SetNum(1);
	 
	ConstructorHelpers::FObjectFinder<UTexture2D> billboardTexture(TEXT("Texture2D'/Engine/EditorResources/Waypoint.Waypoint'"));
	billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Component"));
	billboard->SetSprite(billboardTexture.Object);
	SetRootComponent(billboard);

	spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	spline->EditorUnselectedSplineSegmentColor = FColor(255, 171, 38, 255);

	spline->SetLocationAtSplinePoint(0, FVector(-50, -50, 0), ESplineCoordinateSpace::Local);
	spline->SetLocationAtSplinePoint(1, FVector(0, 0, 0), ESplineCoordinateSpace::Local);
	spline->SetTangentAtSplinePoint(0, FVector(0, 50, 0), ESplineCoordinateSpace::Local);
	spline->SetTangentAtSplinePoint(1, FVector(150, 0, 0), ESplineCoordinateSpace::Local);
}

// Called when the game starts or when spawned
void AParkingSlot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParkingSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

