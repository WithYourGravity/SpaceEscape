// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "EscapePlayer.h"
#include "SpaceEscapeGameModeBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	gm = Cast<ASpaceEscapeGameModeBase>(GetWorld()->GetAuthGameMode());

	if (hpMI)
	{
		hpDMI = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, hpMI);
	}

	PrintCurrentHPPercent();
	PrintCurrentPlayTime();
}

void UPlayerInfoWidget::PrintCurrentHPPercent()
{
	if (player == nullptr)
	{
		return;
	}

	float curHPPer = player->GetHP() / static_cast<float>(player->maxHP);

	if (curHPPer <= 0)
	{
		curHPPer = 0.0f;
	}

	hpDMI->SetScalarParameterValue(FName("percent"), curHPPer);
	curHPPercentImage->SetBrushFromMaterial(hpDMI);
	curHP->SetText(FText::AsNumber(curHPPer * 100));
}

void UPlayerInfoWidget::PrintCurrentPlayTime()
{
	if (gm == nullptr)
	{
		return;
	}

	FNumberFormattingOptions opts;
	opts.MinimumIntegralDigits = 2;
	opts.MaximumIntegralDigits = 2;
	curMinute->SetText(FText::AsNumber(gm->minute, &opts));
	curSecond->SetText(FText::AsNumber(gm->second, &opts));
}
