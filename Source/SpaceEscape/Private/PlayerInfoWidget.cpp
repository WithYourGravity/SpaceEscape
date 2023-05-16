// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "EscapePlayer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	PrintCurrentHP();
	PrintCurrentHPPercent();
}

void UPlayerInfoWidget::PrintCurrentHP()
{
	if (player == nullptr)
	{
		return;
	}

	curHP->SetText(FText::AsNumber(player->GetHP()));
}

void UPlayerInfoWidget::PrintCurrentHPPercent()
{
	if (player == nullptr)
	{
		return;
	}

	curHPPercent->SetPercent(player->GetHP() / static_cast<float>(player->maxHP));
}
