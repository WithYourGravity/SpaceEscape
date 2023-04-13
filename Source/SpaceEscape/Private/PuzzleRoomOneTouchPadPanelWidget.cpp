// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneTouchPadPanelWidget.h"

#include "Components/TextBlock.h"

// 화면에 입력된 비밀번호를 초기화 하는 함수
void UPuzzleRoomOneTouchPadPanelWidget::deleteCurrentScreen()
{
	currentScreen.Empty();
	TextBlock_Number->SetText(FText::FromString(currentScreen));
}

// 현재 화면에 입력된 비밀번호 스트링을 가져오는 함수
FString UPuzzleRoomOneTouchPadPanelWidget::GetCurrentScreen()
{
	return currentScreen;
}

// 현재 입력된 번호를 화면에 추가하는 함수
void UPuzzleRoomOneTouchPadPanelWidget::addCurrentScreen(FString number)
{
	if (currentScreen.Len() < 4)
	{
		currentScreen.Append(number);
		TextBlock_Number->SetText(FText::FromString(currentScreen));
	}
}
