// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "EngineUtils.h"
#include "ResearcherEnemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Tick 함수가 매 프레임 호출되지 않도록 설정
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	// 랜덤 생성 시간 구하기
	float createTime = FMath::RandRange(minTime, maxTime);
	
	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &AEnemyManager::CreateEnemyRoomOne, createTime);
	//GetWorld()->GetTimerManager().SetTimer(spawnTimer2, this, &AEnemyManager::CreateEnemyRoomTwo, createTime);

	// 스폰 위치 동적 할당
	FindSpawnPoints();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreateEnemyRoomOne()
{
	auto spawnEnemy = GetWorld()->SpawnActor<AResearcherEnemy>(enemyCreatureFactory, roomOneSpawnPoints[0]->GetActorLocation(), FRotator(0));

	if (!spawnEnemy)
	{
		float createTime = FMath::RandRange(minTime, maxTime);
		GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &AEnemyManager::CreateEnemyRoomOne, createTime);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(spawnTimer);
	}
}

void AEnemyManager::CreateEnemyRoomTwo()
{
	int index1 = FMath::RandRange(0, roomTwoSpawnPoints.Num() - 1);
	class AActor* temp = roomTwoSpawnPoints[index1];
	roomTwoSpawnPoints[index1] = roomTwoSpawnPoints[roomTwoSpawnPoints.Num() - 1];
	roomTwoSpawnPoints[roomTwoSpawnPoints.Num() - 1] = temp;

	GetWorld()->GetTimerManager().SetTimer(alienSpawnTimer, this, &AEnemyManager::CreateAlien, 1.0f);

	int index2 = FMath::RandRange(0, roomTwoSpawnPoints.Num() - 2);
	temp = roomTwoSpawnPoints[index2];
	roomTwoSpawnPoints[index2] = roomTwoSpawnPoints[roomTwoSpawnPoints.Num() - 2];
	roomTwoSpawnPoints[roomTwoSpawnPoints.Num() - 2] = temp;

	GetWorld()->GetTimerManager().SetTimer(androidSpawnTimer, this, &AEnemyManager::CreateAndroid, 1.0f);
}

void AEnemyManager::CreateAlien()
{
	auto spawnEnemy = GetWorld()->SpawnActor<AResearcherEnemy>(enemyAlienFactory, roomTwoSpawnPoints[roomTwoSpawnPoints.Num() - 1]->GetActorLocation(), FRotator(0));

	if (!spawnEnemy)
	{
		GetWorld()->GetTimerManager().SetTimer(alienSpawnTimer, this, &AEnemyManager::CreateAlien, 1.0f);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(alienSpawnTimer);
	}
}

void AEnemyManager::CreateAndroid()
{
	auto spawnEnemy = GetWorld()->SpawnActor<AResearcherEnemy>(enemyAndroidFactory, roomTwoSpawnPoints[roomTwoSpawnPoints.Num() - 2]->GetActorLocation(), FRotator(0));

	if (!spawnEnemy)
	{
		GetWorld()->GetTimerManager().SetTimer(androidSpawnTimer, this, &AEnemyManager::CreateAndroid, 1.0f);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(androidSpawnTimer);
	}
}

void AEnemyManager::FindSpawnPoints()
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* spawn = *It;
		// 찾은 액터의 이름에 해당 문자열을 포함하고 있다면
		if (spawn->GetName().Contains(TEXT("BP_RoomOneEnemySpawnPoint")))
		{
			// 스폰 목록에 추가
			roomOneSpawnPoints.Add(spawn);
		}
		else if (spawn->GetName().Contains(TEXT("BP_RoomTwoEnemySpawnPoint")))
		{
			roomTwoSpawnPoints.Add(spawn);
		}
	}
}

