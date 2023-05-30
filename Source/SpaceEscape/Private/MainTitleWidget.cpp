// Fill out your copyright notice in the Description page of Project Settings.


#include "MainTitleWidget.h"

#include "EscapePlayer.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UMainTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_GameStart->OnClicked.AddDynamic(this, &UMainTitleWidget::GameStart);
	btn_Credit->OnClicked.AddDynamic(this, &UMainTitleWidget::ShowCredit);
	btn_Exit->OnClicked.AddDynamic(this, &UMainTitleWidget::GameExit);
	btn_CloseCredit->OnClicked.AddDynamic(this, &UMainTitleWidget::CloseCredit);

	// �÷��̾� ���ͷ���
	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (player)
	{
		player->ActiveRightWidgetInteraction();
		player->DeactivateLeftWidgetInteraction();
	}
}

void UMainTitleWidget::GameStart()
{
	DeactivatePlayerInteraction();
	
	UE_LOG(LogTemp, Warning, TEXT("UMainTitleWidget::GameStart"));

	// ������ ���

	//  ������ ������ �÷��̾� ������ġ��, �÷���Ÿ�� ����

}

void UMainTitleWidget::GameExit()
{
	DeactivatePlayerInteraction();

	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UMainTitleWidget::ShowCredit()
{
	switcher->SetActiveWidgetIndex(1);
}

void UMainTitleWidget::CloseCredit()
{
	switcher->SetActiveWidgetIndex(0);
}

void UMainTitleWidget::DeactivatePlayerInteraction()
{
	if (player)
	{
		player->DeactivateLeftWidgetInteraction();
		player->DeactivateRightWidgetInteraction();
	}
}
