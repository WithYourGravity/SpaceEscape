// Fill out your copyright notice in the Description page of Project Settings.


#include "RankingWidget.h"

#include "PuzzleRoomThreeMorse.h"
#include "Ranking.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void URankingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Exit->OnClicked.AddDynamic(this, &URankingWidget::GoToMain);
	btn_Delete->OnClicked.AddDynamic(this, &URankingWidget::DeleteName);
	btn_Enter->OnClicked.AddDynamic(this, &URankingWidget::EnterName);

	rankingActor = Cast<ARanking>(UGameplayStatics::GetActorOfClass(this, ARanking::StaticClass()));
	morse = Cast<APuzzleRoomThreeMorse>(UGameplayStatics::GetActorOfClass(this, APuzzleRoomThreeMorse::StaticClass()));

	nameBlockArray = { text_Name1, text_Name2, text_Name3 };
	timeBlockArray = { text_Time1, text_Time2, text_Time3 };

	SetVisibility(ESlateVisibility::Hidden);
}

void URankingWidget::GoToMain()
{
	UGameplayStatics::OpenLevel(this, FName("FixMapStructure"));
}

void URankingWidget::DeleteName()
{
	// 금요일->delete랑 enter 버튼 무반응!
	morse->GetScreenString().Empty();
}

void URankingWidget::EnterName()
{
	rankingActor->AddToRanking(morse->GetScreenString());
	switcher->SetActiveWidgetIndex(0);
}

