// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkingSlot.h"


bool AParkingSlot::CanCarStartParking()
{
	if (!HasCar())
		return false;
	return FVector::Dist(currentCar->GetActorLocation(), entrySpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World)) < 2;
}

bool AParkingSlot::IsCarCloseEnough()
{
	if (!HasCar())
		return false;
	return GetDistanceTo(currentCar) < 2;
}

bool AParkingSlot::HasCar()
{
	return currentCar != nullptr;
}

bool AParkingSlot::CanCarExit()
{
	if(!HasCar())
		return false;
	return FVector::Dist(currentCar->GetActorLocation(), exitSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World)) < 2;
}

bool AParkingSlot::ParkCar(float time)
{
	if(!HasCar())
		return false;
	currentCar->PauseCar(time);
	return true;
}

FTransform AParkingSlot::GetTransformAtTime(float time, ESplineCoordinateSpace::Type coordinateSpace)
{
	float delta = time - carStartTime;
	if (delta > entrySpline->GetSplineLength())
	{
		return exitSpline->GetTransformAtDistanceAlongSpline(delta - entryTime, ESplineCoordinateSpace::World);
	}
	entryTime = delta;
	return entrySpline->GetTransformAtDistanceAlongSpline(delta, ESplineCoordinateSpace::World);
}

bool AParkingSlot::SetCar(ATrafficCar * car, float time)
{
	if (HasCar())
		return false;
	carStartTime = time;
	state = EParkingState::Arriving;
	currentCar = car;
	return true;
}

ATrafficCar* AParkingSlot::RemoveCar()
{
	ATrafficCar* ret = currentCar;
	currentCar = nullptr;
	state = EParkingState::None;
	return ret;
}

float AParkingSlot::GetDesiredSpeed(float time)
{
	return 0.0f;
}

// Sets default values
AParkingSlot::AParkingSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
#if UE_BUILD_SHIPPING
	PrimaryActorTick.bCanEverTick = false;
#else
	PrimaryActorTick.bCanEverTick = true;  
#endif // UE_BUILD_SHIPPING
	 
	ConstructorHelpers::FObjectFinder<UTexture2D> billboardTexture(TEXT("Texture2D'/Engine/EditorResources/Waypoint.Waypoint'"));
	billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Component"));
	billboard->SetSprite(billboardTexture.Object);
	SetRootComponent(billboard);

	entrySpline = CreateDefaultSubobject<USplineComponent>(TEXT("EntrySpline"));
	exitSpline = CreateDefaultSubobject<USplineComponent>(TEXT("ExitSpline"));

	entrySpline->EditorUnselectedSplineSegmentColor = FColor(255, 171, 38, 255);
	exitSpline->EditorUnselectedSplineSegmentColor = FColor(194, 255, 108, 255);

	entrySpline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	exitSpline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	entrySpline->SetLocationAtSplinePoint(0, FVector(-50, -50, 0), ESplineCoordinateSpace::Local);
	entrySpline->SetLocationAtSplinePoint(1, FVector(0, 0, 0), ESplineCoordinateSpace::Local);
	exitSpline->SetLocationAtSplinePoint(1, FVector(-50, 50, 0), ESplineCoordinateSpace::Local);

	entrySpline->SetTangentAtSplinePoint(1, FVector(50, 0, 0), ESplineCoordinateSpace::Local);
	entrySpline->SetTangentAtSplinePoint(0, FVector(0, 50, 0), ESplineCoordinateSpace::Local);
	exitSpline->SetTangentAtSplinePoint(0, FVector(-50, 0, 0), ESplineCoordinateSpace::Local);
	exitSpline->SetTangentAtSplinePoint(1, FVector(0, 50, 0), ESplineCoordinateSpace::Local);
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

