// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Public/Traffic/TrafficCar.h"

#include "TrafficRoad.h"
#include "TrafficEmitter.h"

// Sets default values
ATrafficCar::ATrafficCar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));
	sensedArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Sensed Area"));

	sensedArea->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TrafficHelper::SetTrafficTransmitterCollision(sensedArea);
}

// Called when the game starts or when spawned
void ATrafficCar::BeginPlay()
{
	Super::BeginPlay();

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
	SCOPE_CYCLE_COUNTER(STAT_CollisionQueries);
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
		road->CarFinished(this, nullptr);
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

void ATrafficCar::PutOnRoad(ATrafficRoad * newRoad, float roadSpeed, float _time)
{
	road = newRoad;
	maxSpeed = speedMultiplier * roadSpeed;
	time = _time;
}

//------------------------------------------------------------------
//------------------------ Traffic Tick ----------------------------
//------------------------------------------------------------------
void ATrafficCar::FTrafficTick(float DeltaT)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Car Traffic Tick"), STAT_CarTrafficTick, STATGROUP_TrafficSystem);
	BeforeTrafficTick(DeltaT);

	// Paused
	if (TickPause(DeltaT))
		return;
	
	int ID = GetUniqueID();

	//Speed
	targetSpeed = road->GetDesiredSpeed(time, ID) * maxSpeed;

	//Traffic slowdown
	TickTrafficSlowdown(targetSpeed, DeltaT);

	//Is obstructed
	TickObstructed(targetSpeed, DeltaT);

	//Compute time
	time += DeltaT * speed;

	//Get new WorldTransform
	FTransform nextTransform = road->GetTransformAtTime(time, ESplineCoordinateSpace::World, ID);

	//Decide sensor direction
	TickSensorDirection(nextTransform, DeltaT);

	//Set WorldTransform
	SetWorldTransformNoScale(nextTransform);

	//End of the road
	TickEndOfTheRoad(ID);
	
	AfterTrafficTick(DeltaT);
}

bool ATrafficCar::TickPause(float DeltaT)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Car Tick Paused"), STAT_CarTick_Paused, STATGROUP_TrafficSystem);
	if (paused)
	{
		if (pauseLimit > 0)
		{
			if (pauseTime < pauseLimit)
			{
				pauseTime += DeltaT;
				return true;
			}
			UnpauseCar();
		}
		else if (pauseLimit == 0)
		{
			UnpauseCar();
			return true;
		}
		else return true;
	}
	return false;
}
void ATrafficCar::TickTrafficSlowdown(float & _targetSpeed, float DeltaT)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Car Tick Traffic Slowdown"), STAT_CarTick_TrafficSlowdown, STATGROUP_TrafficSystem);
	if (SensorArray->IsOverlapping())
	{
		if (_targetSpeed * slowdownMultiplier > SensorArray->GetAvgSpeedOfOverlapCars())
		{
			slowdownMultiplier = FMath::Lerp(slowdownMultiplier, 0.0f, FMath::Clamp(DeltaT * slowdownLerpSpeed, 0.0f, 1.0f));
		}
		else
		{
			slowdownMultiplier = FMath::Lerp(slowdownMultiplier, 1.0f, FMath::Clamp(DeltaT * accelerationLerpSpeed, 0.0f, 1.0f));
		}
		_targetSpeed = _targetSpeed * slowdownMultiplier;
	}
}
void ATrafficCar::TickObstructed(float _targetSpeed, float DeltaT)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Car Tick Obstructed"), STAT_CarTickObstructed, STATGROUP_TrafficSystem);
	if (freePath)
	{
		forcedStopTime = 0;
		speed = FMath::Lerp(speed, _targetSpeed, FMath::Clamp(DeltaT * accelerationLerpSpeed, 0.0f, 1.0f));
	}
	else
	{
		forcedStopTime += DeltaT;
		if (forcedStopTime > forcedStopLimit)
			ResolveDeadlock();
		speed = FMath::Lerp(speed, (float)0, FMath::Clamp(DeltaT * emergencyerpSpeed, 0.0f, 1.0f));
	}
}
void ATrafficCar::TickSensorDirection(FTransform nextTransform, float DeltaT)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Car Tick Sensor Direction"), STAT_CarTick_SensorDirection, STATGROUP_TrafficSystem);
	DeltaYaw = (nextTransform.Rotator().Yaw - GetActorRotation().Yaw) / DeltaT;
	if (FMath::Abs(DeltaYaw) > rearAngle)
	{
		SensorArray->ChangeSensorDirection(ESensorDirection::Rear);
	}
	else
	{
		if (DeltaYaw > 0)
		{
			if (DeltaYaw > sideAngle)
				SensorArray->ChangeSensorDirection(ESensorDirection::Right);
			else if (DeltaYaw > frontSideAngle)
				SensorArray->ChangeSensorDirection(ESensorDirection::FrontRight);
			else
				SensorArray->ChangeSensorDirection(ESensorDirection::Front);
		}
		else
		{
			if (DeltaYaw < -sideAngle)
				SensorArray->ChangeSensorDirection(ESensorDirection::Left);
			else if (DeltaYaw < -frontSideAngle)
				SensorArray->ChangeSensorDirection(ESensorDirection::FrontLeft);
			else
				SensorArray->ChangeSensorDirection(ESensorDirection::Front);
		}
	}
}
void ATrafficCar::TickEndOfTheRoad(int ID)
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Car Tick End of the Road"), STAT_CarTick_EndOfTheRoad, STATGROUP_TrafficSystem);
	if (road->CanLeaveRoad(time, ID))
	{
		if (road->IsLeaf())
		{
			Kill();
		}
		else
			road->CarFinished(this, (forcedProgress < forcedPath.Num() ? forcedPath[++forcedProgress] : nullptr));
	}
}
//-------------------------- Tick END ------------------------------

void ATrafficCar::GetBlockingActors(TSet<AActor*>& actors, TSubclassOf<AActor> classFilter)
{
	SensorArray->GetOverlappingCars(actors, classFilter);
}

bool ATrafficCar::ResolveDeadlock()
{
	SCOPE_CYCLE_COUNTER(STAT_CollisionQueries);
	TArray<ATrafficCar*> deadlockCars;
	TSet<AActor*> tempOverlaps;
	ATrafficCar* tmpCar;
	deadlockCars.Add(this);
	for (int i = 0; i < deadlockCars.Num(); i++)
	{
		deadlockCars[i]->GetBlockingActors(tempOverlaps, ATrafficCar::StaticClass());
		for (AActor* a : tempOverlaps)
		{
			tmpCar = Cast<ATrafficCar>(a);
			if (deadlockCars.Contains(tmpCar))
			{
				Kill();
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
