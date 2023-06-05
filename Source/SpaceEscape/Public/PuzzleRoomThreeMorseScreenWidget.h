// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzleRoomThreeMorseScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UPuzzleRoomThreeMorseScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UTextBlock* TextBlock_Morse;
	UPROPERTY()
	class UWidgetSwitcher* switcher;
};
