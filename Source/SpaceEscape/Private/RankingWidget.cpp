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
	btn_DeleteEng->OnClicked.AddDynamic(this, &URankingWidget::DeleteName);
	btn_Enter->OnClicked.AddDynamic(this, &URankingWidget::EnterName);
	btn_EnterEng->OnClicked.AddDynamic(this, &URankingWidget::EnterName);

	rankingActor = Cast<ARanking>(UGameplayStatics::GetActorOfClass(this, ARanking::StaticClass()));
	morse = Cast<APuzzleRoomThreeMorse>(UGameplayStatics::GetActorOfClass(this, APuzzleRoomThreeMorse::StaticClass()));

	nameBlockArray = { text_Name1, text_Name2, text_Name3, text_Name4, text_Name5, text_Name6, text_Name7, text_Name8, text_Name9 };
	timeBlockArray = { text_Time1, text_Time2, text_Time3, text_Time4, text_Time5, text_Time6, text_Time7, text_Time8, text_Time9 };

	SetVisibility(ESlateVisibility::Hidden);

	clickSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/UIClick.UIClick'")));
}

// ���� ������ ������ؼ� ������������ �̵��ϴ� �Լ�
void URankingWidget::GoToMain()
{
	PlayClickSound();
	UGameplayStatics::OpenLevel(this, FName("FinalMap"));
}

// ��ŷ�� �Է����̾��� �̸��� ��ü�����ϴ� �Լ�
void URankingWidget::DeleteName()
{
	PlayClickSound();
	morse->EmptyScreenString();
	morse->setScreenText(morse->GetScreenString());
}

// ��ŷ�� �Է��� �̸��� ��ŷ���Ϳ� �Ѱ��ְ� ��ŷ�� �����ְ� ��������ġ�� �۵���Ű�� �Լ�
void URankingWidget::EnterName()
{
	PlayClickSound();
	rankingActor->AddToRanking(morse->GetScreenString());
	switcher->SetActiveWidgetIndex(0);
}

void URankingWidget::PlayClickSound()
{
	UGameplayStatics::PlaySound2D(this, clickSound);
}

