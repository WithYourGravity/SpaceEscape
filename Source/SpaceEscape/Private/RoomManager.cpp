// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"
#include "EngineUtils.h"
#include "PuzzleBase.h"

// Sets default values
ARoomManager::ARoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARoomManager::BeginPlay()
{
	Super::BeginPlay();

	// 퍼즐이 클리어 되면 델리게이트 바인딩된 함수 실행
	for(TActorIterator<APuzzleBase> it(GetWorld()); it; ++it)
	{
		APuzzleBase* pz = *it;
		pz->puzzleClearDele.BindUFunction(this, FName("AddSolvedPuzzleCount"));
	}
}

// Called every frame
void ARoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 완료한 퍼즐 개수 올리고 다 풀었다면 다음 스테이지로 넘어가는 함수
void ARoomManager::AddSolvedPuzzleCount()
{
	solvedPuzzleCount++;

	if (playingStage >= 1 && playingStage <= 3 && solvedPuzzleCount == totalPuzzlePerStage[playingStage])
	{
		MoveOnNextStage();
	}

	//UE_LOG(LogTemp, Warning, TEXT("solved pulzze count is : %d"), solvedPuzzleCount);
	//UE_LOG(LogTemp, Warning, TEXT("current stage is : %d"), playingStage);
}

// 다음 스테이지로 넘어가며 퍼즐카운트 초기화 해주는 함수
void ARoomManager::MoveOnNextStage()
{
	playingStage++;
	solvedPuzzleCount = 0;
	stageClearDele.Execute();
}

// 현재 스테이지에서 문을 열 수 있는지 여부를 리턴하는 함수
bool ARoomManager::GetbCanOpenDoor()
{
	return bCanOpenDoor;
}

// 다음 스테이지로 넘어갈 때 다시 문 잠그는 함수
void ARoomManager::SetbCanOpenDoor(bool canOpenDoor)
{
	bCanOpenDoor = canOpenDoor;
}

