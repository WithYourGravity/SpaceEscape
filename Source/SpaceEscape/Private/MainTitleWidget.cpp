// Fill out your copyright notice in the Description page of Project Settings.


#include "MainTitleWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_GameStart->OnClicked.AddDynamic(this, &UMainTitleWidget::GameStart);
	btn_Credit->OnClicked.AddDynamic(this, &UMainTitleWidget::ShowCredit);
	btn_Exit->OnClicked.AddDynamic(this, &UMainTitleWidget::GameExit);
}

void UMainTitleWidget::GameStart()
{
	UE_LOG(LogTemp, Warning, TEXT("UMainTitleWidget::GameStart"));

	// ������ ���

	//  ������ ������ �÷��̾� ������ġ��, �÷���Ÿ�� ����

}

void UMainTitleWidget::GameExit()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UMainTitleWidget::ShowCredit()
{
	UE_LOG(LogTemp, Warning, TEXT("UMainTitleWidget::ShowCredit"));
}
