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
		float parkTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float parkTimeVariance;
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

	void AddCar(ATrafficCar* newCar) override;
	bool IsLeaf() override;
	void DetachCar(ATrafficCar* car) override;
	void Initialize_Implementation() override;
	FTransform GetTransformAtTime(float time, ESplineCoordinateSpace::Type splineCoordinateSpaceType, int carID) override;
	float GetDesiredSpeed(float time, int carID) override;
	bool CanLeaveRoad(float time, int carID) override;
	void FTrafficTick(float DeltaT) override;
};

