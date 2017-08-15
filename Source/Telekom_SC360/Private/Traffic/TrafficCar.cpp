// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Public/Traffic/TrafficCar.h"

// Sets default values
ATrafficCar::ATrafficCar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));
	sensingArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Sensing Area"));
	sensedArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Sensed Area"));

	sensingArea->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	sensedArea->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TrafficHelper::SetTrafficReceiverCollision(sensingArea);
	TrafficHelper::SetTrafficTransmitterCollision(sensedArea);
}

// Called when the game starts or when spawned
void ATrafficCar::BeginPlay()
{
	Super::BeginPlay();
	
	sensingArea->OnComponentBeginOverlap.AddDynamic(this, &ATrafficCar::OnSensingBoxBeginOverlap);
	sensingArea->OnComponentEndOverlap.AddDynamic(this, &ATrafficCar::OnSensingBoxEndOverlap);
}

// Called every frame
void ATrafficCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ATrafficCar::GetUniqueID_Int()
{
	return GetUniqueID();
}

bool ATrafficCar::IsStopped(float tolerance)
{
	return FMath::IsNearlyEqual(speed, 0.0f, tolerance);
}

void ATrafficCar::Despawn()
{
	SetActorHiddenInGame(true);
	const TArray<FOverlapInfo> OverlapInfoCopy(sensedArea->GetOverlapInfos());
	for (const FOverlapInfo OtherOverlap : OverlapInfoCopy)
		sensedArea->EndComponentOverlap(OtherOverlap, true, true);
	sensedArea->bGenerateOverlapEvents = false;
}

void ATrafficCar::Respawn(FTransform spawnPoint)
{
	OnRespawn(spawnPoint);
	SetActorHiddenInGame(false);
	sensedArea->bGenerateOverlapEvents = true;
	forcedProgress = 1;
	freePath = true;
	SetWorldTransformNoScale(spawnPoint);
}

void ATrafficCar::Kill()
{
	if (road)
		road->DetachCar(this);
	emitter->CarFinished(this);
}

void ATrafficCar::PauseCar(float duration)
{
	paused = true;
	pauseLimit = duration;
	pauseTime = 0;
}

void ATrafficCar::UnpauseCar()
{
	paused = false;
}

void ATrafficCar::PutOnRoad(ATrafficRoad * newRoad, float roadSpeed)
{
	road = newRoad;
	maxSpeed = speedMultiplier * roadSpeed;
	time = 0;
}

void ATrafficCar::FTrafficTick(float DeltaT)
{
	BeforeTrafficTick(DeltaT);

	// Paused
	if (paused)
	{
		if (pauseLimit > 0)
		{
			if (pauseTime > pauseLimit)
			{
				pauseTime += DeltaT;
				return;
			}
			UnpauseCar();
		}
		else if (pauseLimit == 0)
		{
			UnpauseCar();
			return;
		}
		else return;
	}
	
	int ID = GetUniqueID();

	//Speed
	targetSpeed = road->GetDesiredSpeed(time, ID) * maxSpeed;

	//Is obstructed
	if (freePath)
	{
		forcedStopTime = 0;
		speed = FMath::Lerp(speed, targetSpeed, FMath::Clamp(DeltaT * accelerationLerpSpeed, float(0), float(1)));
	}
	else
	{
		forcedStopTime += DeltaT;
		if (forcedStopTime > forcedStopLimit)
			ResolveDeadlock();
		speed = FMath::Lerp(speed, (float)0, FMath::Clamp(DeltaT * decelerationLerpSpeed, float(0), float(1)));
	}
	time += DeltaT * speed;
	SetWorldTransformNoScale(road->GetTransformAtTime(time, ESplineCoordinateSpace::World, ID));

	//End of the road
	if (road->CanLeaveRoad(time, ID))
	{
		if (road->IsLeaf())
		{
			Kill();
		}
		else
			road->CarFinished(this, (forcedProgress < forcedPath.Num() ? forcedPath[++forcedProgress] : nullptr));
	}
	
	AfterTrafficTick(DeltaT);
}

void ATrafficCar::OnSensingBoxBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this)
		return;
	ATrafficCar* otherCar = Cast<ATrafficCar>(OtherActor);
	if (otherCar != nullptr && !otherCar->sensingArea->IsOverlappingComponent(sensedArea))
	{
		overlapCount++;
		freePath = false;
	}
}

void ATrafficCar::OnSensingBoxEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this)
		return;
	if (Cast<ATrafficCar>(OtherActor) != nullptr)
	{
		overlapCount--;
		if (overlapCount <= 0)
		{
			freePath = true;
			overlapCount = 0;
		}
	}
}

bool ATrafficCar::ResolveDeadlock()
{
	TArray<ATrafficCar*> deadlockCars;
	TArray<AActor*> tempOverlaps;
	ATrafficCar* tmpCar;
	deadlockCars.Add(this);
	for (int i = 0; i < deadlockCars.Num(); i++)
	{
		deadlockCars[i]->sensingArea->GetOverlappingActors(tempOverlaps, ATrafficCar::StaticClass());
		for (AActor* a : tempOverlaps)
		{
			tmpCar = Cast<ATrafficCar>(a);
			if (deadlockCars.Contains(tmpCar))
			{
				tmpCar->road->DetachCar(tmpCar);
				tmpCar->emitter->CarFinished(tmpCar);
				return true;
			}
			deadlockCars.Add(tmpCar);
		}
	}
	return false;
}

void ATrafficCar::Bump(float size)
{
	time += size;
}

void ATrafficCar::SetWorldTransformNoScale(FTransform transform)
{
	RootComponent->SetWorldLocation(transform.GetLocation());
	RootComponent->SetWorldRotation(transform.GetRotation());
}
