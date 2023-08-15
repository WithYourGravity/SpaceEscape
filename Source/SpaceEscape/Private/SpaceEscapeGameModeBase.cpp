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

	// 코엑스 시연시 한글을 기본값으로
	ChangeLanguageSetting(ELanguageSettings::KOREAN);
}

// 플레이타임 카운트하는 함수
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

// 플레이타임 카운트를 멈추는 함수
void ASpaceEscapeGameModeBase::StopPlayTime()
{
	GetWorld()->GetTimerManager().ClearTimer(countTimer);
}

// 플레이타임 카운트를 시작하는 함수
void ASpaceEscapeGameModeBase::StartPlayTime()
{
	GetWorld()->GetTimerManager().SetTimer(countTimer, this, &ASpaceEscapeGameModeBase::CountPlayTime, 1.0f, true, 0);
}

// 스테이지를 입력하면 바로 그 스테이지부터 시작하게 해주는 함수(시연용)
void ASpaceEscapeGameModeBase::SetStage(int stage)
{
	// For Steam
	//return;

	// 시연용 코드
	if (GetWorldTimerManager().IsTimerActive(player->dialogueTimer))
	{
		GetWorldTimerManager().ClearTimer(player->dialogueTimer);
	}

	switch (stage)
	{
	case 0:
		UGameplayStatics::OpenLevel(this, FName("FinalMap"));
		break;
	case 1 :
		if (rm->GetCurrentPlayingStage() != 0.5f)
		{
			return;
		}
		rm->MoveOnNextStage();
		player->SetActorLocation(FVector(-6100, -2430, 80));
		player->DeactivateRightWidgetInteraction();
		StartPlayTime();
		break;
	case 2:
		if (rm->GetCurrentPlayingStage() != 0.5f)
		{
			return;
		}
		while (rm->GetCurrentPlayingStage() < 2)
		{
			rm->MoveOnNextStage();
		}
		player->SetActorLocation(FVector(-3500, -3520, 130));
		player->DeactivateRightWidgetInteraction();
		StartPlayTime();
		break;
	case 3:
		if (rm->GetCurrentPlayingStage() != 0.5f)
		{
			return;
		}
		while (rm->GetCurrentPlayingStage() < 3)
		{
			rm->MoveOnNextStage();
		}
		player->SetActorLocation(FVector(-3415, -600, 1350));
		player->DeactivateRightWidgetInteraction();
		StartPlayTime();
		break;
	}
}

void ASpaceEscapeGameModeBase::ChangeLanguageSetting(ELanguageSettings language)
{
	currentLanguageSetting = language;
	changeLanguageDele.Broadcast(language);
}
