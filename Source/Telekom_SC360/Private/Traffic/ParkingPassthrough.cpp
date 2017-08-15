// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkingPassthrough.h"

bool AParkingPassthrough::IsSlotAssigned(int slot)
{
	return (assignedFlags & ((uint64)1 << slot)) != 0;
}

void AParkingPassthrough::AssignSlot(int slot, ATrafficCar* car)
{
	assignedSlots.Add(car->GetUniqueID(), slot);
	assignedFlags |= ((uint64)1 << slot);
	UE_LOG(TrafficLog, Log, TEXT("ASSIGNED slot %d to %s"), slot, *car->GetName());
}

void AParkingPassthrough::FreeSlot(int slot)
{
	assignedFlags &= !((uint64)1 << slot);
	parkingSlots[slot]->DetachCar(nullptr);
	assignedSlots.Remove(*assignedSlots.FindKey(slot));
	UE_LOG(TrafficLog, Log, TEXT("FREE slot %d"), slot);
}

TArray<int> AParkingPassthrough::GetAssignedSlots()
{
	TArray<int> ret;
	int j = 0;
	for (uint64 i = 1; j < size; i <<= 1)
	{
		if (assignedFlags & i)
		{
			ret.Add(j);
		}
		j++;
	}
	return ret;
}

TArray<int> AParkingPassthrough::GetFreeSlots()
{
	TArray<int> ret;
	int j = 0;
	for (uint64 i = 1; j < size; i <<= 1)
	{
		if (!(assignedFlags & i))
		{
			ret.Add(j);
		}
		j++;
	}
	return ret;
}

int AParkingPassthrough::GetNumFreeSlots()
{
	uint64 x = assignedFlags;
	int count;
	for (count = 0; x != 0; count++)
	{
		x &= x - 1;
	}
	return count;
}

void AParkingPassthrough::AddCarFromSlot(ATrafficCar * newCar)
{
	currentCars.Add(newCar);
	newCar->PutOnRoad(this, baseSpeed + FMath::RandRange(-speedVariance, speedVariance), parkingSlots[assignedSlots[newCar->GetUniqueID()]]->carStartTime);
	FreeSlot(assignedSlots[newCar->GetUniqueID()]);
	UE_LOG(TrafficLog, Log, TEXT("ADD %s from slot to %s"), *newCar->GetName(), *GetName());
}

void AParkingPassthrough::AddCar(ATrafficCar * newCar)
{
	Super::AddCar(newCar);
	TArray<int> freeSlots = GetFreeSlots();
	if (freeSlots.Num() == 0)
		return;
	AssignSlot(freeSlots[FMath::RandRange(0, freeSlots.Num() - 1)], newCar);
}

bool AParkingPassthrough::IsLeaf()
{
	return false;
}

void AParkingPassthrough::DetachCar(ATrafficCar * car)
{
	FreeSlot(assignedSlots[car->GetUniqueID()]);
	Super::DetachCar(car);
}

void AParkingPassthrough::Initialize_Implementation()
{
	length = spline->GetSplineLength();
	size = parkingSlots.Num();
	for (AParkingSlot* slot : parkingSlots)
	{
		slot->Initialize();
	}
}

FTransform AParkingPassthrough::GetTransformAtTime(float time, ESplineCoordinateSpace::Type splineCoordinateSpaceType, int carID)
{
	return Super::GetTransformAtTime(time, splineCoordinateSpaceType, carID);
}

float AParkingPassthrough::GetDesiredSpeed(float time, int carID)
{
	return Super::GetDesiredSpeed(time, carID);
}

bool AParkingPassthrough::CanLeaveRoad(float time, int carID)
{
	return Super::CanLeaveRoad(time, carID);
}

void AParkingPassthrough::FTrafficTick(float DeltaT)
{
	BeforeTrafficTick(DeltaT);
	for (ATrafficCar* car : currentCars)
	{
		car->FTrafficTick(DeltaT);

		int ID = car->GetUniqueID();
		if (assignedSlots.Contains(ID))
		{
			AParkingSlot* currentSlot = parkingSlots[assignedSlots[ID]];
			if (currentSlot->CanCarStartParking(car->GetActorLocation()))
			{
				currentSlot->AddCar(car);
				removeList.Add(car);
			}
			else
			{
				car->FTrafficTick(DeltaT);
			}
		}
	}
	for (ATrafficCar* removeCar : removeList)
	{
		currentCars.Remove(removeCar);
	}
	removeList.Empty();
	for (AParkingSlot* slot : parkingSlots)
	{
		slot->FTrafficTick(DeltaT);
	}
	/*for (auto& slotIdx : assignedSlots)
	{
		parkingSlots[slotIdx.Value]->FTrafficTick(DeltaT);
	}*/
	AfterTrafficTick(DeltaT);
}

void AParkingPassthrough::CarFinished(ATrafficCar * car, ATrafficRoad * forcedRoad)
{
	int ID = car->GetUniqueID();
	if (assignedSlots.Contains(ID))
		FreeSlot(assignedSlots[ID]);
	else
		UE_LOG(TrafficLog, Log, TEXT("Unassigned car finished"));
	Super::CarFinished(car, forcedRoad);
}
