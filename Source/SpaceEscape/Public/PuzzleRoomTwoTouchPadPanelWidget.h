// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzleRoomTwoTouchPadPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UPuzzleRoomTwoTouchPadPanelWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	class UTextBlock* TextBlock_Number;

	FString GetCurrentScreen();
	void deleteCurrentScreen();
	void addCurrentScreen(FString number);

private:
	FString currentScreen;
};
