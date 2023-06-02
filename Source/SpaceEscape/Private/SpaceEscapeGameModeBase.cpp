// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceEscapeGameModeBase.h"
#include "EscapePlayer.h"
#include "PlayerInfoWidget.h"
#include "RoomManager.h"
#include "Kismet/GameplayStatics.h"

void ASpaceEscapeGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));
}

// �÷���Ÿ�� ī��Ʈ�ϴ� �Լ�
void ASpaceEscapeGameModeBase::CountPlayTime()
{
	second++;

	if (second == 60)
	{
		minute++;
		second = 0;
	}

	if (player && player->infoUI)
	{
		player->infoUI->PrintCurrentPlayTime();
	}
}

// �÷���Ÿ�� ī��Ʈ�� ���ߴ� �Լ�
void ASpaceEscapeGameModeBase::StopPlayTime()
{
	GetWorld()->GetTimerManager().ClearTimer(countTimer);
}

// �÷���Ÿ�� ī��Ʈ�� �����ϴ� �Լ�
void ASpaceEscapeGameModeBase::StartPlayTime()
{
	GetWorld()->GetTimerManager().SetTimer(countTimer, this, &ASpaceEscapeGameModeBase::CountPlayTime, 1.0f, true, 0);
}

// ���������� �Է��ϸ� �ٷ� �� ������������ �����ϰ� ���ִ� �Լ�
void ASpaceEscapeGameModeBase::SetStage(int stage)
{
	UGameplayStatics::OpenLevel(this, FName("FinalMap"));
	rm->MoveOnNextStage();
	StartPlayTime();

	switch (stage)
	{
	case 1 :
		player->SetActorLocation(FVector(-6100, -2430, 80));
		break;
	case 2:
		rm->MoveOnNextStage();
		rm->MoveOnNextStage();
		player->SetActorLocationAndRotation(FVector(-3500, -3520, 130), FRotator(0, -90, 0));
		break;
	case 3:
		rm->MoveOnNextStage();
		rm->MoveOnNextStage();
		rm->MoveOnNextStage();
		rm->MoveOnNextStage();
		player->SetActorLocationAndRotation(FVector(-3415, -600, 1350), FRotator(0, 90, 0));
		break;
	}
}
