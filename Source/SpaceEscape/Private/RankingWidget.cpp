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

	nameBlockArray = { text_Name1, text_Name2, text_Name3, text_Name4, text_Name5, text_Name6, text_Name7, text_Name8, text_Name9 };
	timeBlockArray = { text_Time1, text_Time2, text_Time3, text_Time4, text_Time5, text_Time6, text_Time7, text_Time8, text_Time9 };

	SetVisibility(ESlateVisibility::Hidden);
}

// 현재 레벨을 재오픈해서 메인위젯으로 이동하는 함수
void URankingWidget::GoToMain()
{
	UGameplayStatics::OpenLevel(this, FName("FixLastRoom_Yeni"));
}

// 랭킹에 입력중이었던 이름을 전체삭제하는 함수
void URankingWidget::DeleteName()
{
	morse->EmptyScreenString();
	morse->setScreenText(morse->GetScreenString());
}

// 랭킹에 입력한 이름을 랭킹액터에 넘겨주고 랭킹을 보여주게 위젯스위치를 작동시키는 함수
void URankingWidget::EnterName()
{
	rankingActor->AddToRanking(morse->GetScreenString());
	switcher->SetActiveWidgetIndex(0);
}

