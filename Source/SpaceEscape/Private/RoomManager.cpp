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

	// ������ Ŭ���� �Ǹ� ��������Ʈ ���ε��� �Լ� ����
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

// �Ϸ��� ���� ���� �ø��� �� Ǯ���ٸ� ���� ���������� �Ѿ�� �Լ�
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

// ���� ���������� �Ѿ�� ����ī��Ʈ �ʱ�ȭ ���ִ� �Լ�
void ARoomManager::MoveOnNextStage()
{
	playingStage++;
	solvedPuzzleCount = 0;
	stageClearDele.Execute();
}

// ���� ������������ ���� �� �� �ִ��� ���θ� �����ϴ� �Լ�
bool ARoomManager::GetbCanOpenDoor()
{
	return bCanOpenDoor;
}

// ���� ���������� �Ѿ �� �ٽ� �� ��״� �Լ�
void ARoomManager::SetbCanOpenDoor(bool canOpenDoor)
{
	bCanOpenDoor = canOpenDoor;
}

