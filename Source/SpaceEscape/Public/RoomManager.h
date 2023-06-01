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
	// ���� �÷��� ���� �ܰ�
	float playingStage = 0.5;
	// ���� �ܰ迡�� �Ϸ��� ���� ����
	int solvedPuzzleCount = 0;
	// �� �ܰ躰 �Ϸ��ؾ��ϴ� ���� ������ ��� ��
	TMap<float, int> totalPuzzlePerStage = {{1, 2}, {2, 2}, {3, 2}};
	// ���� �ܰ迡�� ���� ���� �ִ���
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

	// sense ����
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

	// sequence play ����
	UPROPERTY()
	class ULevelSequencePlayer* sequencePlayer;
	UPROPERTY(EditAnywhere)
	class ULevelSequence* sequenceAsset;
	UPROPERTY()
	class ALevelSequenceActor* sequenceActor;
};
