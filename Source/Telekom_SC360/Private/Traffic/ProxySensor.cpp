// Fill out your copyright notice in the Description page of Project Settings.

#include "ProxySensor.h"

AProxySensor::AProxySensor()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
}

bool AProxySensor::IsProxy()
{
	return true;
}

void AProxySensor::BeginPlay()
{
}

void AProxySensor::Tick(float DeltaTime)
{
}
