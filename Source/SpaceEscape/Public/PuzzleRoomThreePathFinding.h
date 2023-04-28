// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "PuzzleRoomThreePathFinding.generated.h"

USTRUCT()
struct FNodeInfo
{
	GENERATED_BODY()
public:
	int nodeIndex;
	int parentNodeIndex;
	float gValue;
	float hValue;
	float fValue;
};

UCLASS()
class SPACEESCAPE_API APuzzleRoomThreePathFinding : public APuzzleBase
{
	GENERATED_BODY()

public:
	APuzzleRoomThreePathFinding();

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* sceneComp;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void PickBoxRandomly(int number);
	void MovingTrigger();
	void MovingFunctionAtTick(float deltaTime);
	void ResetBeginAndEndPoint();
	void ResetThisPuzzle();

private:

	TArray<class UStaticMeshComponent*> groundBoxArray;
	TArray<int> selectedBoxIndexArray;
	TArray<FVector> startLocArray;

	int width = 10;
	int length = 20;
	int NumberOfPopUpBox = width * length * 0.25;
	int countForRecordStartLoc;
	int beginPointIndex = -1;
	int endPointIndex = -1;
	bool bIsMoving;
	float zPos;
	float lerpTime;


	int regularlyUpCount;
	FTimerHandle hd;


	// 알고리즘 부분

	TArray<FNodeInfo> possibleNodeList;
	TArray<FNodeInfo> pickedNodeList;
	TArray<int> AnswerPathArray;
	FTimerHandle answerPathHandle;
	bool FindPossibleNode(int currentNodeIndex);
	bool CheckMovable(int nextNodeIndex);
	void AddIndexPossibleList(int nodeIndex);
	void AddBestIndexPickedList();
	int GetYourParentIndex(int nodeIndex);
	bool LetsFindPath();
	void PathLight();

	int currentNodeIndex;
	int parentIndex;
	int answerPathIndex;
};
