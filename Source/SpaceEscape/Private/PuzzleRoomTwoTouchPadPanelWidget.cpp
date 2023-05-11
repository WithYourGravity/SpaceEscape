// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoTouchPadPanelWidget.h"

#include "Components/TextBlock.h"

// ȭ�鿡 �Էµ� ��й�ȣ�� �ʱ�ȭ �ϴ� �Լ�
void UPuzzleRoomTwoTouchPadPanelWidget::deleteCurrentScreen()
{
	currentScreen.Empty();
	TextBlock_Number->SetText(FText::FromString(currentScreen));
}

// ���� ȭ�鿡 �Էµ� ��й�ȣ ��Ʈ���� �������� �Լ�
FString UPuzzleRoomTwoTouchPadPanelWidget::GetCurrentScreen()
{
	return currentScreen;
}

// ���� �Էµ� ��ȣ�� ȭ�鿡 �߰��ϴ� �Լ�
void UPuzzleRoomTwoTouchPadPanelWidget::addCurrentScreen(FString number)
{
	if (currentScreen.Len() < 4)
	{
		currentScreen.Append(number);
		TextBlock_Number->SetText(FText::FromString(currentScreen));
	}
}