// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomManager.generated.h"

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
	int playingStage = 1;
	// ���� �ܰ迡�� �Ϸ��� ���� ����
	int solvedPuzzleCount;
	// �� �ܰ躰 �Ϸ��ؾ��ϴ� ���� ������ ��� ��
	TMap<int, int> totalPuzzlePerStage = {{1, 2}, {2, 3}, {3, 3}};
	// ���� �ܰ迡�� ���� ���� �ִ���
	bool bCanOpenDoor;

public:
	UFUNCTION()
	void AddSolvedPuzzleCount();
	void MoveOnNextStage();
	bool GetbCanOpenDoor();
	void SetbCanOpenDoor(bool canOpenDoor);

};
