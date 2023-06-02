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
	// 랜덤 시간 간격 최소값
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	float minTime = 1.0f;
	// 랜덤 시간 간격 최대값
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	float maxTime = 2.0f;
	// 스폰할 위치 정보 배열
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	TArray<class AActor*> roomOneSpawnPoints;
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
	TArray<class AActor*> roomTwoSpawnPoints;
	// 적 블루프린트 할당 변수
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

	// 적 생성 함수
	void CreateEnemyRoomOne();
	void CreateEnemyRoomTwo();

	void CreateAlien();
	void CreateAndroid();

	// 스폰할 위치 동적으로 찾아 할당
	void FindSpawnPoints();

	UFUNCTION()
	void SpawnEnemy();
};
