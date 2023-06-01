// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class SPACEESCAPE_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ���� �ð� ���� �ּҰ�
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	float minTime = 1.0f;
	// ���� �ð� ���� �ִ밪
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	float maxTime = 2.0f;
	// ������ ��ġ ���� �迭
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	TArray<class AActor*> roomOneSpawnPoints;
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	TArray<class AActor*> roomTwoSpawnPoints;
	// �� �������Ʈ �Ҵ� ����
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	TSubclassOf<class AResearcherEnemy> enemyAlienFactory;
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	TSubclassOf<class AResearcherEnemy> enemyCreatureFactory;
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	TSubclassOf<class AResearcherEnemy> enemyAndroidFactory;

	FTimerHandle spawnTimer;
	FTimerHandle alienSpawnTimer;
	FTimerHandle androidSpawnTimer;

	int32 enemyCount = 0;

	UPROPERTY()
	class ARoomManager* roomManager;

	// �� ���� �Լ�
	void CreateEnemyRoomOne();
	void CreateEnemyRoomTwo();

	void CreateAlien();
	void CreateAndroid();

	// ������ ��ġ �������� ã�� �Ҵ�
	void FindSpawnPoints();

	UFUNCTION()
	void SpawnEnemy();
};
