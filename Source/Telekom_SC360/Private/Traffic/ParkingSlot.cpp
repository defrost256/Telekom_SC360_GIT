// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkingSlot.h"


bool AParkingSlot::CanCarStartParking()
{
	if (!HasCar() || state != EParkingState::Arriving)
		return false;
	return FVector::Dist(currentCars[0]->GetActorLocation(), spline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World)) < 2;
}

bool AParkingSlot::IsCarCloseEnough()
{
	if (!HasCar())
		return false;
	return GetDistanceTo(currentCars[0]) < 2;
}

bool AParkingSlot::HasCar()
{
	return currentCars[0] != nullptr;
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
	return FVector::Dist(currentCars[0]->GetActorLocation(), spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World)) < 2;
}

void AParkingSlot::FTrafficTick(float DeltaT)
{
	BeforeTrafficTick(DeltaT);
	if (HasCar())
	{
		if (IsCarCloseEnough())
		{
			currentCars[0]->PauseCar(parkTime + FMath::RandRange(0.0f, parkTimeVariance));
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
	currentCars[0] = nullptr;
	state = EParkingState::None;
}

void AParkingSlot::Initialize_Implementation()
{
	Super::Initialize_Implementation();
}

// Sets default values
AParkingSlot::AParkingSlot()
{
	PrimaryActorTick.bCanEverTick = false;

	currentCars.SetNum(1);
	 
	ConstructorHelpers::FObjectFinder<UTexture2D> billboardTexture(TEXT("Texture2D'/Engine/EditorResources/Waypoint.Waypoint'"));
	billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Component"));
	billboard->SetSprite(billboardTexture.Object);
	SetRootComponent(billboard);
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

