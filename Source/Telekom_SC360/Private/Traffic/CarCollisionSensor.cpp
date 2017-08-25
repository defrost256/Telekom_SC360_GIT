// Fill out your copyright notice in the Description page of Project Settings.

#include "CarCollisionSensor.h"

#include "TrafficCar.h"

// Sets default values
ACarCollisionSensor::ACarCollisionSensor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	FrontSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Front Sensor"));
	FrontRightSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Front Right Sensor"));
	FrontLeftSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Front Left Sensor"));
	RightSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Sensor"));
	LeftSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Sensor"));
	RearSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Rear Sensor"));
	EmergencySensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Emergency Sensor"));

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
}

void ACarCollisionSensor::OnSensorBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	SCOPE_CYCLE_COUNTER(STAT_CollisionQueries);
	if (Cast<ATrafficCar>(OtherActor) == MyCar)
		return;
	overlapCount++;
}

void ACarCollisionSensor::OnSensorEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	SCOPE_CYCLE_COUNTER(STAT_CollisionQueries);
	if (Cast<ATrafficCar>(OtherActor) == MyCar)
		return;
	overlapCount--;
}

void ACarCollisionSensor::OnEmergencySensorBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	SCOPE_CYCLE_COUNTER(STAT_CollisionQueries);
	ATrafficCar* otherCar = Cast<ATrafficCar>(OtherActor);
	if (otherCar == MyCar)
		return;
	if (otherCar != nullptr)
	{
		emergencyOverlap++;
	}
}

void ACarCollisionSensor::OnEmergencySensorEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	SCOPE_CYCLE_COUNTER(STAT_CollisionQueries);
	if (OtherActor == this)
		return;
	if (Cast<ATrafficCar>(OtherActor) != nullptr)
	{
		emergencyOverlap--;
		if (emergencyOverlap <= 0)
		{
			emergencyOverlap = 0;
		}
	}
}

UBoxComponent * ACarCollisionSensor::GetSensorByDirection(ESensorDirection dir)
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

UBoxComponent * ACarCollisionSensor::GetCurrentSensor()
{
	return GetSensorByDirection(activeDirection);
}

float ACarCollisionSensor::GetAvgSpeedOfOverlapCars()
{
	SCOPE_CYCLE_COUNTER(STAT_CollisionQueries);
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
	/*if (cars != overlapCount)
	UE_LOG(TrafficLog, Log, TEXT("ERROR - overlapped car count doesn't match ATrafficCar::overlapCount"));*/
	return acc / cars;
}

void ACarCollisionSensor::ChangeSensorDirection(ESensorDirection newDirection)
{
	if (newDirection == activeDirection)
		return;
	GetSensorByDirection(activeDirection)->bGenerateOverlapEvents = false;
	GetSensorByDirection(newDirection)->bGenerateOverlapEvents = true;
	activeDirection = newDirection;
	overlapCount = 0;
}

void ACarCollisionSensor::GetOverlappingCars(TSet<AActor*>& outActors, TSubclassOf<AActor> classFilter, bool emergency)
{
	if (emergency)
	{
		GetCurrentSensor()->GetOverlappingActors(outActors, classFilter);
	}
	else
	{
		EmergencySensor->GetOverlappingActors(outActors, classFilter);
	}
}

FString ACarCollisionSensor::GetCurrentSensorDirectionName()
{
	return GetSensorDirectionName(activeDirection);
}

bool ACarCollisionSensor::IsOverlapping()
{
	return overlapCount > 0;
}

void ACarCollisionSensor::AssignCar(ATrafficCar * car)
{
	Super::AssignCar(car);
}

void ACarCollisionSensor::RemoveCar()
{
	Super::RemoveCar();
}

// Called when the game starts or when spawned
void ACarCollisionSensor::BeginPlay()
{
	Super::BeginPlay();

	FrontSensor->OnComponentBeginOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorBeginOverlap);
	FrontSensor->OnComponentEndOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorEndOverlap);
	FrontRightSensor->OnComponentBeginOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorBeginOverlap);
	FrontRightSensor->OnComponentEndOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorEndOverlap);
	FrontLeftSensor->OnComponentBeginOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorBeginOverlap);
	FrontLeftSensor->OnComponentEndOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorEndOverlap);
	RightSensor->OnComponentBeginOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorBeginOverlap);
	RightSensor->OnComponentEndOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorEndOverlap);
	LeftSensor->OnComponentBeginOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorBeginOverlap);
	LeftSensor->OnComponentEndOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorEndOverlap);
	RearSensor->OnComponentBeginOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorBeginOverlap);
	RearSensor->OnComponentEndOverlap.AddDynamic(this, &ACarCollisionSensor::OnSensorEndOverlap);

	FrontSensor->bGenerateOverlapEvents = false;
	FrontRightSensor->bGenerateOverlapEvents = false;
	FrontLeftSensor->bGenerateOverlapEvents = false;
	RightSensor->bGenerateOverlapEvents = false;
	LeftSensor->bGenerateOverlapEvents = false;
	RearSensor->bGenerateOverlapEvents = false;
	
	EmergencySensor->OnComponentBeginOverlap.AddDynamic(this, &ACarCollisionSensor::OnEmergencySensorBeginOverlap);
	EmergencySensor->OnComponentEndOverlap.AddDynamic(this, &ACarCollisionSensor::OnEmergencySensorEndOverlap);

	ChangeSensorDirection(ESensorDirection::Front);

}

// Called every frame
void ACarCollisionSensor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

