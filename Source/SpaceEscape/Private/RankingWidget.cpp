// Fill out your copyright notice in the Description page of Project Settings.


#include "RankingWidget.h"

#include "Components/Button.h"

void URankingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Exit->OnClicked.AddDynamic(this, &URankingWidget::GoToMain);
	btn_Delete->OnClicked.AddDynamic(this, &URankingWidget::DeleteName);
	btn_Enter->OnClicked.AddDynamic(this, &URankingWidget::EnterName);
}

void URankingWidget::GoToMain()
{
	
}

void URankingWidget::DeleteName()
{

}

void URankingWidget::EnterName()
{

}

