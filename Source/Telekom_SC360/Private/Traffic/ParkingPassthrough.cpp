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
	parkingSlots[slot]->SetCar(car, car->time);
}

void AParkingPassthrough::FreeSlot(int slot)
{
	assignedFlags |= !((uint64)1 << slot);
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

	}
}

FTransform AParkingPassthrough::GetTransformAtTime(float time, ESplineCoordinateSpace::Type splineCoordinateSpaceType, int carID)
{
	if (!assignedSlots.Contains(carID))
	{
		return spline->GetTransformAtDistanceAlongSpline(time, splineCoordinateSpaceType);
	}
	AParkingSlot* currentSlot = parkingSlots[assignedSlots[carID]];
	switch (currentSlot->state)
	{
	case EParkingState::Arriving:
		return spline->GetTransformAtDistanceAlongSpline(time, splineCoordinateSpaceType);
	case EParkingState::Parking:
		return currentSlot->GetTransformAtTime(time, splineCoordinateSpaceType);
	case EParkingState::Leaving:
		return spline->GetTransformAtDistanceAlongSpline(time, splineCoordinateSpaceType);
	default:
		break;
	}
	return FTransform();
}

float AParkingPassthrough::GetDesiredSpeed(float time, int carID)
{
	if (!assignedSlots.Contains(carID))
	{
		return (speedCurve ? speedCurve->GetFloatValue(time) : 1) * baseSpeed;
	}
	AParkingSlot* currentSlot = parkingSlots[assignedSlots[carID]];
	if (currentSlot->state == EParkingState::Parking)
	{
		return currentSlot->GetDesiredSpeed(time);
	}
	return (speedCurve ? speedCurve->GetFloatValue(time) : 1) * baseSpeed;
}

bool AParkingPassthrough::CanLeaveRoad(float time, int carID)
{
	return false;
}

void AParkingPassthrough::FTrafficTick(float DeltaT)
{
	BeforeTrafficTick(DeltaT);
	for (ATrafficCar* car : currentCars)
	{
		int ID = car->GetUniqueID();
		if (assignedSlots.Contains(ID))
		{
			AParkingSlot* currentSlot = parkingSlots[assignedSlots[ID]];
			switch (currentSlot->state)
			{
			case EParkingState::Arriving:
				if (currentSlot->CanCarStartParking())
				{
					currentSlot->state = EParkingState::Parking;
				}
				break;
			case EParkingState::Parking:
				if (currentSlot->CanCarExit())
				{
					currentSlot->state = EParkingState::Leaving;
				}
				break;
			case EParkingState::Leaving:
				break;
			default:
				return;
			}
		}
		car->FTrafficTick(DeltaT);
	}
	AfterTrafficTick(DeltaT);
}
