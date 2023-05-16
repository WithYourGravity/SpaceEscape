// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzleRoomOneTouchPadPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UPuzzleRoomOneTouchPadPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	class UTextBlock* TextBlock_Number;
	UPROPERTY(EditDefaultsOnly)
	class UTextBlock* TextBlock_word;
	UPROPERTY(EditDefaultsOnly)
	class UImage* Image_back;

	FString GetCurrentScreen();
	void deleteCurrentScreen();
	void addCurrentScreen(FString number);

private:
	FString currentScreen;
};
