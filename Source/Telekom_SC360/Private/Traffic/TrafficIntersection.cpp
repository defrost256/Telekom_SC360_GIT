// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficIntersection.h"


// Sets default values
ATrafficIntersection::ATrafficIntersection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
}

// Called when the game starts or when spawned
void ATrafficIntersection::BeginPlay()
{
	Super::BeginPlay();
	
	UTrafficSignGroup* currentGroup;
	for (FTrafficSignGroupParam g : signGroupParams)
	{
		for (ATrafficSign* sign : g.phaseOneSign)
		{
			sign->Initialize();
			if (g.startingPhase == ETrafficSignGroupPhase::PhaseOne)
				sign->StartTraffic();
			else
				sign->StopTraffic();
		}
		for (ATrafficSign* sign : g.phaseTwoSign)
		{
			sign->Initialize();
			if (g.startingPhase == ETrafficSignGroupPhase::PhaseTwo)
				sign->StartTraffic();
			else
				sign->StopTraffic();
		}
		UE_LOG(TrafficLog, Log, TEXT("Initializing new TrafficSignGroup"));
		currentGroup = NewObject<UTrafficSignGroup>();
		UE_LOG(TrafficLog, Log, TEXT("Initialized new TrafficSignGroup"));
		currentGroup->Initialize(g);
		signGroups.Add(currentGroup);
	}
}

// Called every frame
void ATrafficIntersection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (UTrafficSignGroup* signGroup : signGroups)
	{
		signGroup->timer += DeltaTime;
		if (signGroup->currentPhase == ETrafficSignGroupPhase::None && signGroup->timer > 0)
		{
			signGroup->currentPhase = signGroup->nextPhase;
			signGroup->nextPhase = ETrafficSignGroupPhase::None;
			for (ATrafficSign* sign : (signGroup->currentPhase == ETrafficSignGroupPhase::PhaseOne ? signGroup->phaseOneSign : signGroup->phaseTwoSign))
			{
				sign->StartTraffic();
			}
		}
		else if (signGroup->timer > signGroup->DelayBetweenSwitch)
		{
			signGroup->timer = -signGroup->DelayBeforeSwitch;
			signGroup->nextPhase = (signGroup->currentPhase == ETrafficSignGroupPhase::PhaseOne ? ETrafficSignGroupPhase::PhaseTwo : ETrafficSignGroupPhase::PhaseOne);
			for (ATrafficSign* sign : (signGroup->currentPhase == ETrafficSignGroupPhase::PhaseOne ? signGroup->phaseOneSign : signGroup->phaseTwoSign))
			{
				sign->StopTraffic();
			}
			signGroup->currentPhase = ETrafficSignGroupPhase::None;
		}
	}
}

