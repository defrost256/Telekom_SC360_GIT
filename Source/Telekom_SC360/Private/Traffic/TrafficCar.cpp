// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Public/Traffic/TrafficCar.h"

// Sets default values
ATrafficCar::ATrafficCar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));
	sensedArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Sensed Area"));

	FrontSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Front Sensor"));
	FrontRightSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Front Right Sensor"));
	FrontLeftSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Front Left Sensor"));
	RightSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Sensor"));
	LeftSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Sensor"));
	RearSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Rear Sensor"));
	EmergencySensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Emergency Sensor"));

	sensedArea->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	FrontSensor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FrontRightSensor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FrontLeftSensor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FrontSensor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FrontSensor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FrontSensor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	EmergencySensor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TrafficHelper::SetTrafficReceiverCollision(FrontSensor);
	TrafficHelper::SetTrafficReceiverCollision(FrontRightSensor);
	TrafficHelper::SetTrafficReceiverCollision(FrontLeftSensor);
	TrafficHelper::SetTrafficReceiverCollision(RightSensor);
	TrafficHelper::SetTrafficReceiverCollision(LeftSensor);
	TrafficHelper::SetTrafficReceiverCollision(RearSensor);
	TrafficHelper::SetTrafficReceiverCollision(EmergencySensor);

	TrafficHelper::SetTrafficTransmitterCollision(sensedArea);
}

// Called when the game starts or when spawned
void ATrafficCar::BeginPlay()
{
	Super::BeginPlay();
	
	FrontSensor->OnComponentBeginOverlap.AddDynamic(this, &ATrafficCar::OnSensorBeginOverlap);
	FrontSensor->OnComponentEndOverlap.AddDynamic(this, &ATrafficCar::OnSensorEndOverlap);
	FrontRightSensor->OnComponentBeginOverlap.AddDynamic(this, &ATrafficCar::OnSensorBeginOverlap);
	FrontRightSensor->OnComponentEndOverlap.AddDynamic(this, &ATrafficCar::OnSensorEndOverlap);
	FrontLeftSensor->OnComponentBeginOverlap.AddDynamic(this, &ATrafficCar::OnSensorBeginOverlap);
	FrontLeftSensor->OnComponentEndOverlap.AddDynamic(this, &ATrafficCar::OnSensorEndOverlap);
	RightSensor->OnComponentBeginOverlap.AddDynamic(this, &ATrafficCar::OnSensorBeginOverlap);
	RightSensor->OnComponentEndOverlap.AddDynamic(this, &ATrafficCar::OnSensorEndOverlap);
	LeftSensor->OnComponentBeginOverlap.AddDynamic(this, &ATrafficCar::OnSensorBeginOverlap);
	LeftSensor->OnComponentEndOverlap.AddDynamic(this, &ATrafficCar::OnSensorEndOverlap);
	RearSensor->OnComponentBeginOverlap.AddDynamic(this, &ATrafficCar::OnSensorBeginOverlap);
	RearSensor->OnComponentEndOverlap.AddDynamic(this, &ATrafficCar::OnSensorEndOverlap);

	EmergencySensor->OnComponentBeginOverlap.AddDynamic(this, &ATrafficCar::OnEmergencySensorBeginOverlap);
	EmergencySensor->OnComponentEndOverlap.AddDynamic(this, &ATrafficCar::OnEmergencySensorEndOverlap);
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
	BeforeTrafficTick(DeltaT);

	// Paused
	if (paused)
	{
		if (pauseLimit > 0)
		{
			if (pauseTime < pauseLimit)
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

	//Traffic slowdown
	if (overlapCount > 0)
	{
		if (targetSpeed * slowdownMultiplier > GetAvgSpeedOfOverlapCars())
		{
			slowdownMultiplier = FMath::Lerp(slowdownMultiplier, 0.0f, DeltaT);
		}
		else
		{
			slowdownMultiplier = FMath::Lerp(slowdownMultiplier, 1.0f, DeltaT);
		}
		targetSpeed = targetSpeed * slowdownMultiplier;
	}

	//Is obstructed
	if (freePath)
	{
		forcedStopTime = 0;
		speed = FMath::Lerp(speed, targetSpeed, FMath::Clamp(DeltaT * accelerationLerpSpeed, 0.0f, 1.0f));
	}
	else
	{
		forcedStopTime += DeltaT;
		if (forcedStopTime > forcedStopLimit)
			ResolveDeadlock();
		speed = FMath::Lerp(speed, (float)0, FMath::Clamp(DeltaT * decelerationLerpSpeed, 0.0f, 1.0f));
	}

	//Compute time
	time += DeltaT * speed;

	//Get new WorldTransform
	FTransform nextTransform = road->GetTransformAtTime(time, ESplineCoordinateSpace::World, ID);

	//Decide sensor direction
	float DeltaYaw = nextTransform.Rotator().Yaw - GetActorRotation().Yaw;
	if (FMath::Abs(DeltaYaw) > 120)
	{
		ChangeSensorDirection(ESensorDirection::Rear);
	}
	else
	{
		if (DeltaYaw > 0)
		{
			if (DeltaYaw > 60)
				ChangeSensorDirection(ESensorDirection::Right);
			else if (DeltaYaw > 30)
				ChangeSensorDirection(ESensorDirection::FrontRight);
			else
				ChangeSensorDirection(ESensorDirection::Front);
		}
		else
		{
			if (DeltaYaw < -60)
				ChangeSensorDirection(ESensorDirection::Left);
			else if (DeltaYaw < -30)
				ChangeSensorDirection(ESensorDirection::FrontLeft);
			else
				ChangeSensorDirection(ESensorDirection::Front);
		}
	}

	//Set WorldTransform
	SetWorldTransformNoScale(nextTransform);

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
//-------------------------- Tick END ------------------------------


void ATrafficCar::OnSensorBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this)
		return;
	overlapCount++;
}

void ATrafficCar::OnSensorEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this)
		return;
	overlapCount--;
}

void ATrafficCar::OnEmergencySensorBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this)
		return;
	ATrafficCar* otherCar = Cast<ATrafficCar>(OtherActor);
	if (otherCar != nullptr && !otherCar->GetCurrentSensor()->IsOverlappingComponent(sensedArea))
	{
		emergencyOverlap++;
		freePath = false;
	}
}

void ATrafficCar::OnEmergencySensorEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this)
		return;
	if (Cast<ATrafficCar>(OtherActor) != nullptr)
	{
		emergencyOverlap--;
		if (emergencyOverlap <= 0)
		{
			freePath = true;
			emergencyOverlap = 0;
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
		deadlockCars[i]->GetCurrentSensor()->GetOverlappingActors(tempOverlaps, ATrafficCar::StaticClass());
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

UBoxComponent * ATrafficCar::GetSensorByDirection(ESensorDirection dir)
{
	switch (dir)
	{
	case ESensorDirection::Front:
		return FrontSensor;
	case ESensorDirection::FrontRight:
		return FrontRightSensor;
	case ESensorDirection::FrontLeft:
		return FrontLeftSensor;
	case ESensorDirection::Right:
		return RightSensor;
	case ESensorDirection::Left:
		return LeftSensor;
	case ESensorDirection::Rear:
		return RearSensor;
	default:
		return nullptr;
	}
	return nullptr;
}

void ATrafficCar::ChangeSensorDirection(ESensorDirection newDirection)
{
	if (newDirection == activeDirection)
		return;
	GetSensorByDirection(activeDirection)->bGenerateOverlapEvents = false;
	GetSensorByDirection(newDirection)->bGenerateOverlapEvents = true;
	overlapCount = 0;
}

UBoxComponent * ATrafficCar::GetCurrentSensor()
{
	return GetSensorByDirection(activeDirection);
}

float ATrafficCar::GetAvgSpeedOfOverlapCars()
{
	UBoxComponent* CurrentSensor = GetCurrentSensor();
	ATrafficCar* OtherCar;
	const TArray<FOverlapInfo> OverlapInfoCopy(CurrentSensor->GetOverlapInfos());
	float acc = 0.0f;
	int cars = 0;
	for (const FOverlapInfo OtherOverlap : OverlapInfoCopy)
	{
		OtherCar = Cast<ATrafficCar>(OtherOverlap.OverlapInfo.GetActor());
		if (OtherCar)
		{
			acc += OtherCar->speed;
			cars++;
		}
	}
	if (cars != overlapCount)
		UE_LOG(TrafficLog, Log, TEXT("ERROR - overlapped car count doesn't match ATrafficCar::overlapCount"));
	return acc / cars;
}

void ATrafficCar::SetWorldTransformNoScale(FTransform transform)
{
	RootComponent->SetWorldLocation(transform.GetLocation());
	RootComponent->SetWorldRotation(transform.GetRotation());
}
