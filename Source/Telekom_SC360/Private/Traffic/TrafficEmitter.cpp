// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Public/Traffic/TrafficEmitter.h"

// Sets default values
ATrafficEmitter::ATrafficEmitter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UTexture2D> billboardTexture(TEXT("/Engine/EditorResources/S_Emitter.S_Emitter"));
	root = CreateDefaultSubobject<UBillboardComponent>(TEXT("RootComponent"));
	root->SetSprite(billboardTexture.Object);
	SetRootComponent(root);

	startArea = CreateDefaultSubobject<UBoxComponent>(TEXT("StartArea"));
	startArea->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TrafficHelper::SetTrafficReceiverCollision(startArea);
	startArea->SetCanEverAffectNavigation(false);
	startArea->OnComponentBeginOverlap.AddDynamic(this, &ATrafficEmitter::OnStartAreaBeginOverlap);
	startArea->OnComponentEndOverlap.AddDynamic(this, &ATrafficEmitter::OnStartAreaEndOverlap);
}

// Called when the game starts or when spawned
void ATrafficEmitter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrafficEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrafficEmitter::SpawnCar()
{
	ATrafficCar* randomCar;
	if (HasReserves())
	{
		randomCar = carsReserve[FMath::RandRange(0, carsReserve.Num() - 1)];
		carsReserve.RemoveSingleSwap(randomCar);
	}
	else
	{
		randomCar = (ATrafficCar*)(GetWorld()->SpawnActor(*(carPrefabs[FMath::RandRange(0, carPrefabs.Num() - 1)])));
		if (FMath::FRand() < forcePathRatio)
			randomCar->forcedPath.Append(forcedPath);
		randomCar->emitter = this;
	}
	randomCar->Respawn(GetStartTransform());
	sensorPool->AssignSensor(randomCar);
	rootRoad->AddCar(randomCar);
	carsActive.Add(randomCar);
}

bool ATrafficEmitter::HasReserves()
{
	return carsReserve.Num() > 0;
}

FTransform ATrafficEmitter::GetStartTransform()
{
	return rootRoad->spline->GetTransformAtSplinePoint(0, ESplineCoordinateSpace::World);
}

void ATrafficEmitter::CarFinished(ATrafficCar * car)
{
	carsActive.RemoveSingleSwap(car);
	sensorPool->RevokeSensor(car);
	car->Despawn();
	carsReserve.Add(car);
}

void ATrafficEmitter::FTrafficTick(float DeltaT)
{
	BeforeTrafficTick(DeltaT);

	if (!hasCarInSpawn) {
		timeToNextSpawn -= DeltaT;
		if (timeToNextSpawn < 0)
		{
			SpawnCar();
			timeToNextSpawn = spawnFrequency + FMath::RandRange(-spawnVariance, spawnVariance);
		}
	}

	AfterTrafficTick(DeltaT);
}

void ATrafficEmitter::OnStartAreaBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	hasCarInSpawn = true;
}

void ATrafficEmitter::OnStartAreaEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	hasCarInSpawn = false;
}

int ATrafficEmitter::KillPercentOfCars(float killPercent)
{
	int killCounter = 0;
	for (ATrafficCar* car : carsActive)
	{
		if (FMath::FRand() < killPercent)
		{
			car->road->DetachCar(car);
			this->CarFinished(car);
			killCounter++;
		}
	}
	return killCounter;
}

void ATrafficEmitter::Initialize_Implementation()
{
	hasCarInSpawn = false;
	timeToNextSpawn = spawnFrequency + FMath::RandRange(-spawnVariance, spawnVariance);
	for (int i = includeRoads.Num() - 1; i >= 0; i--)
	{
		if (i == 0)
			forcedPath.Append(rootRoad->GetPathTo(includeRoads[i]));
		else
			forcedPath.Append(includeRoads[i - 1]->GetPathTo(includeRoads[i]));
	}
	//Reverse the A* Graph
	ATrafficRoad* temp;
	int reverseIdx;
	for (int i = 0; i < forcedPath.Num() / 2; i++)
	{
		reverseIdx = forcedPath.Num() - i - 1;
		temp = forcedPath[i];
		forcedPath[i] = forcedPath[reverseIdx];
		forcedPath[reverseIdx] = temp;
	}
}
