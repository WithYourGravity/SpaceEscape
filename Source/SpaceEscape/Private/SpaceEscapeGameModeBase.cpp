// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceEscapeGameModeBase.h"
#include "EscapePlayer.h"
#include "PlayerInfoWidget.h"
#include "Kismet/GameplayStatics.h"

void ASpaceEscapeGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

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

void ASpaceEscapeGameModeBase::StopPlayTime()
{
	GetWorld()->GetTimerManager().ClearTimer(countTimer);
}

void ASpaceEscapeGameModeBase::StartPlayTime()
{
	GetWorld()->GetTimerManager().SetTimer(countTimer, this, &ASpaceEscapeGameModeBase::CountPlayTime, 1.0f, true, 0);
}
