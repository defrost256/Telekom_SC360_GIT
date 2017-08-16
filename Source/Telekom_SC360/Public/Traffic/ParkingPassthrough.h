// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Traffic/TrafficRoad.h"
#include "Traffic/TrafficCar.h"
#include "Traffic/ParkingSlot.h"

#include "ParkingPassthrough.generated.h"

class AParkingSlot;

/**
 * ParkingPassthrough
 */
UCLASS()
class TELEKOM_SC360_API AParkingPassthrough : public ATrafficRoad
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float parkingChance;
		
	UPROPERTY(BlueprintReadOnly)
		TMap<int, int> assignedSlots;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<AParkingSlot*> parkingSlots;
	UPROPERTY(BlueprintReadOnly)
		int size;

protected:
	uint64 assignedFlags;

public:

	UFUNCTION(BLueprintCallable, Category = "Traffic|PPassthrough")
		bool IsSlotAssigned(int slot);
	UFUNCTION(BLueprintCallable, Category = "Traffic|PPassthrough")
		void AssignSlot(int slot, ATrafficCar* car);
	UFUNCTION(BLueprintCallable, Category = "Traffic|PPassthrough")
		void FreeSlot(int slot);
	UFUNCTION(BLueprintCallable, Category = "Traffic|PPassthrough")
		TArray<int> GetAssignedSlots();
	UFUNCTION(BLueprintCallable, Category = "Traffic|PPassthrough")
		TArray<int> GetFreeSlots();
	UFUNCTION(BLueprintCallable, Category = "Traffic|PPassthrough")
		int GetNumFreeSlots();
	UFUNCTION(BLueprintCallable, Category = "Traffic|PPassthrough")
		virtual void AddCarFromSlot(ATrafficCar* newCar);


	virtual void AddCar(ATrafficCar* newCar) override;
	virtual bool IsLeaf() override;
	virtual void DetachCar(ATrafficCar* car) override;
	virtual void Initialize_Implementation() override;
	virtual FTransform GetTransformAtTime(float time, ESplineCoordinateSpace::Type splineCoordinateSpaceType, int carID) override;
	virtual float GetDesiredSpeed(float time, int carID) override;
	virtual bool CanLeaveRoad(float time, int carID) override;
	virtual void FTrafficTick(float DeltaT) override;
	virtual void CarFinished(ATrafficCar* car, ATrafficRoad* forcedRoad) override;

};

