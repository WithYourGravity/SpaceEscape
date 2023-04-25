// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "PuzzleRoomThreePathFinding.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API APuzzleRoomThreePathFinding : public APuzzleBase
{
	GENERATED_BODY()

public:
	APuzzleRoomThreePathFinding();

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* sceneComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox1;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox2;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox3;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox4;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox5;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox6;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox7;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox8;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox9;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox10;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox11;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox12;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox13;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox14;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox15;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox16;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox17;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox18;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox19;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox20;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox21;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox22;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox23;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox24;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox25;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox26;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox27;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox28;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox29;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox30;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox31;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox32;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox33;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox34;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox35;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox36;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox37;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox38;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox39;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox40;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox41;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox42;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox43;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox44;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox45;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox46;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox47;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox48;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox49;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* groundBox50;
	UPROPERTY(EditDefaultsOnly)
	class UTimelineComponent* timelineComp;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void PickBoxRandomly(int number);
	void MovingTrigger();
	void AddAllBoxCompToArray();
	void MovingFunctionAtTick(float deltaTime);

private:

	UPROPERTY()
	TArray<class UStaticMeshComponent*> groundBoxArray;
	TArray<int> selectedBoxIndexArray;
	TArray<FVector> startLocArray;
	int NumberOfPopUpBox = 7;
	int countForRecordStartLoc;
	bool bIsMoving;
	float zPos;
	float lerpTime;


	int regularlyUpCount;
	FTimerHandle hd;
};
