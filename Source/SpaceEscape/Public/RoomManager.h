// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FStageClearDele);
DECLARE_MULTICAST_DELEGATE(FGameClearDele);

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
	int playingStage = 1;
	// 현재 단계에서 완료한 퍼즐 개수
	int solvedPuzzleCount;
	// 각 단계별 완료해야하는 퍼즐 개수가 담긴 맵
	TMap<int, int> totalPuzzlePerStage = {{1, 2}, {2, 2}, {3, 2}};
	// 현재 단계에서 문을 열수 있는지
	bool bCanOpenDoor;

public:
	UFUNCTION()
	void AddSolvedPuzzleCount();
	void MoveOnNextStage();
	int GetCurrentPlayingStage();
	int GetSolvedPuzzleCount();
	FStageClearDele stageClearDele;
	FGameClearDele gameClearDele;

	// sense 관련
	UPROPERTY()
	class AEscapePlayer* player;
	void SenseOn();
	void SenseOff();
	void GetInteractionObjectToArray();
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
};
