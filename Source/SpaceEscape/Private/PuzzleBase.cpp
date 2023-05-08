// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleBase.h"

// Sets default values
APuzzleBase::APuzzleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APuzzleBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APuzzleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ���� Ŭ���� �� ��������Ʈ ���� �����ϴ� �Լ�
void APuzzleBase::ReportClear()
{
	if (!bReportClearOnce)
	{
		puzzleClearDele.Broadcast();
		bReportClearOnce = true;
	}
}

