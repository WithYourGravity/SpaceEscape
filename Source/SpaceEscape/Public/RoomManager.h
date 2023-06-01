// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FStageClearDele);
DECLARE_MULTICAST_DELEGATE(FGameClearDele);
DECLARE_MULTICAST_DELEGATE(FEndingDele);
DECLARE_MULTICAST_DELEGATE(FSpawnEnemyDele);

UCLASS()
class SPACEESCAPE_API ARoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// 현재 플레이 중인 단계
	float playingStage = 0.5;
	// 현재 단계에서 완료한 퍼즐 개수
	int solvedPuzzleCount = 0;
	// 각 단계별 완료해야하는 퍼즐 개수가 담긴 맵
	TMap<float, int> totalPuzzlePerStage = {{1, 2}, {2, 2}, {3, 2}};
	// 현재 단계에서 문을 열수 있는지
	bool bCanOpenDoor;

	UPROPERTY()
	class ASpaceShip* ship;
	UPROPERTY()
	class AEscapePlayer* player;
	UPROPERTY()
	class ASpaceEscapeGameModeBase* gm;

public:
	UFUNCTION()
	void AddSolvedPuzzleCount();
	UFUNCTION()
	void SequenceFinished();
	void MoveOnNextStage();
	void StageProgressChecker();
	float GetCurrentPlayingStage();
	int GetSolvedPuzzleCount();
	FStageClearDele stageClearDele;
	FGameClearDele gameClearDele;
	FEndingDele endingDele;
	FSpawnEnemyDele spawnEnemyDele;

	// sense 관련
	void SenseOn();
	void SenseOff();
	void GetInteractionObjectToArray();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActorComponent> smComp;
	UPROPERTY()
	TArray<UStaticMeshComponent*> arrSenseR1Comp;
	UPROPERTY()
	TArray<UStaticMeshComponent*> arrSenseR2Comp;
	UPROPERTY()
	TArray<UStaticMeshComponent*> arrSenseR3Comp;
	UPROPERTY()
	TArray<UStaticMeshComponent*> arrSenseR4Comp;
	UPROPERTY()
	TArray<UStaticMeshComponent*> arrSenseAlwaysComp;

	// sequence play 관련
	UPROPERTY()
	class ULevelSequencePlayer* sequencePlayer;
	UPROPERTY(EditAnywhere)
	class ULevelSequence* sequenceAsset;
	UPROPERTY()
	class ALevelSequenceActor* sequenceActor;
};
