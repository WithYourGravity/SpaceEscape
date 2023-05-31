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

UENUM()
enum class EMoveDir : uint8
{
	Forward,
	Back,
	Left,
	Right
};

UCLASS()
class SPACEESCAPE_API APuzzleRoomThreePathFinding : public APuzzleBase
{
	GENERATED_BODY()

public:
	APuzzleRoomThreePathFinding();

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* sceneComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* backPlateMesh;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void PickBoxRandomly(int number);
	void MovingTrigger();
	void MovingFunctionAtTick(float deltaTime);
	void ResetBeginAndEndPoint();
	void ResetThisPuzzle();
	void ResetRegularlyUpCountForResetPuzzle();
	bool GetbisMoving();
	

private:

	TArray<class UStaticMeshComponent*> groundBoxArray;
	TArray<int> selectedBoxIndexArray;
	TArray<FVector> startLocArray;

	int width = 20;
	int length = 14;
	int NumberOfPopUpBox = width * length * 0.3;
	int countForRecordStartLoc;
	int beginPointIndex = -1;
	int endPointIndex = -1;
	int regularlyUpCount;
	bool bIsMoving;
	bool bUpAndDown;
	bool bVisibility;
	float zPos;
	float lerpTime;
	float blockMoveSpeed = 2;
	
	FTimerHandle resetHandle;
	FLinearColor myRed = FLinearColor(0.25, 0.06, 0.07, 1);
	FLinearColor myGreen = FLinearColor(0.1, 0.24, 0.1, 1);
	FLinearColor myBlue = FLinearColor(0.07, 0.095, 0.24, 1);
	FLinearColor myYellow = FLinearColor(0.5, 0.5, 0, 1);
	FLinearColor myWhite = FLinearColor(1, 1, 1, 1);

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

public:

	// 조이스틱과 연동 부분
	TArray<int> PlayedPathArray;
	bool CheckPlayable(EMoveDir direction);
	void MovePlayingNode(EMoveDir direction);
	int playingNodeIndex;

	// 퍼즐완료 효과 부분
	void EndingEffect();
	void EndingMovingAtTick(float deltatime);
	void EndingMakePathAtTick(float deltatime);
	float forEndingLerp;
	int countForRecordRandLoc;
	bool bEndingMoveTrigger;
	bool bMakePathTrigger;
	bool bDeleteOnce;
	TArray<FVector> newRandLoc;
	TArray<FVector> endingOriginLoc;
	UPROPERTY()
	class APuzzleRoomThreePathLocation* pathLoc;
	UPROPERTY()
	class USoundBase* successSound;
	UPROPERTY()
	class USoundBase* failSound;

};
