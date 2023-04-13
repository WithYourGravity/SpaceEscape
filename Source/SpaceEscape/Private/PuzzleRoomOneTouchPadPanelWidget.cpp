// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneTouchPadPanelWidget.h"

#include "Components/TextBlock.h"

// ȭ�鿡 �Էµ� ��й�ȣ�� �ʱ�ȭ �ϴ� �Լ�
void UPuzzleRoomOneTouchPadPanelWidget::deleteCurrentScreen()
{
	currentScreen.Empty();
	TextBlock_Number->SetText(FText::FromString(currentScreen));
}

// ���� ȭ�鿡 �Էµ� ��й�ȣ ��Ʈ���� �������� �Լ�
FString UPuzzleRoomOneTouchPadPanelWidget::GetCurrentScreen()
{
	return currentScreen;
}

// ���� �Էµ� ��ȣ�� ȭ�鿡 �߰��ϴ� �Լ�
void UPuzzleRoomOneTouchPadPanelWidget::addCurrentScreen(FString number)
{
	if (currentScreen.Len() < 4)
	{
		currentScreen.Append(number);
		TextBlock_Number->SetText(FText::FromString(currentScreen));
	}
}
