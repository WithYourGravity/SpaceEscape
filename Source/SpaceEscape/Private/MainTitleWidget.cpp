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

	// 시퀀스 재생

	//  시퀀스 끝나면 플레이어 시작위치로, 플레이타임 시작

}

void UMainTitleWidget::GameExit()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UMainTitleWidget::ShowCredit()
{
	UE_LOG(LogTemp, Warning, TEXT("UMainTitleWidget::ShowCredit"));
}
