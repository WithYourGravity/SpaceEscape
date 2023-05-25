// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "EngineUtils.h"
#include "ResearcherEnemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Tick �Լ��� �� ������ ȣ����� �ʵ��� ����
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �ð� ���ϱ�
	float createTime = FMath::RandRange(minTime, maxTime);

	//GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &AEnemyManager::CreateEnemy, createTime);

	// ���� ��ġ ���� �Ҵ�
	FindSpawnPoints();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreateEnemy()
{
	if (enemyCount >= 10)
	{
		return;
	}

	// ���� ��ġ
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);
	// �� ���� �� ��ġ
	auto spawnEnemy = GetWorld()->SpawnActor<AResearcherEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0));

	// �ٽ� �����ð��� CreateEnemy �Լ� ȣ��ǵ��� Ÿ�̸� ����
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &AEnemyManager::CreateEnemy, createTime);

	if (spawnEnemy)
	{
		enemyCount++;
	}
}

void AEnemyManager::FindSpawnPoints()
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* spawn = *It;
		// ã�� ������ �̸��� �ش� ���ڿ��� �����ϰ� �ִٸ�
		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			// ���� ��Ͽ� �߰�
			spawnPoints.Add(spawn);
		}
	}
}

