// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Selection.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/Components/SplineComponent.h"

#include "Editor/UnrealEd/Public/Editor.h"

#include "TrafficDataCollectorInterface.h"
#include "TrafficProbeRenderComponent.h"
#include "TrafficRoad.h"

#include "TrafficProbe.generated.h"

UCLASS()
class TELEKOM_SC360_API ATrafficProbe : public AActor, public ITrafficDataCollectorInterface
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Traffic|SpeedCurve")
		bool bDebugSpeedCurve;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Traffic")
		bool bAutoTargetRoad;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Traffic")
		AActor* targetRoad;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Traffic|SpeedCurve")
		FColor SpeedCurveDebugColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Traffic")
		bool bUpdateOnlyOnFinishedMove = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Traffic")
		bool bVisibleInGame = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Traffic")
		bool bTickDuringGame = false;


	UPROPERTY(BlueprintReadWrite)
		FTrafficData currentData;

	UFUNCTION(BlueprintCallable, Category = "Traffic|Probing")
		FVector4 FindLocationAndDistanceAlongSplineNearLocation(USplineComponent* spline, FVector loc, float precision = 0.0001);

	// Sets default values for this actor's properties
	ATrafficProbe();
	ATrafficProbe(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITORONLY_DATA
private:

	UPROPERTY(Transient)
		UTrafficProbeRenderComponent* RenderComp;
#endif //WITH_EDITORONLY_DATA
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif //WITH_EDITOR

	//---------------- ITrafficDataCollectorInterface IMPLEMENTATION
	const FTrafficData* GetTrafficData() const override;
	//---------------- ITrafficDataCollectorInterface END

protected:


	void RunQueries();
	void UpdateDrawing();

	ATrafficRoad* GetNearestRoad();

	static void OnEditorSelectionChanged(UObject* NewSelection);

#if WITH_EDITORONLY_DATA
	/** Returns RenderComp subobject **/
	UTrafficProbeRenderComponent* GetEdRenderComp() { return RenderComp; }
#endif //WITH_EDITORONLY_DATA
};
