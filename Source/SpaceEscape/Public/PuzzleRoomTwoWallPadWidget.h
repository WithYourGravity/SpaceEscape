// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzleRoomTwoWallPadWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UPuzzleRoomTwoWallPadWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	class UTextBlock* textBlock_Activate;
	UPROPERTY(EditDefaultsOnly)
	class UImage* image_Activate;

	void GravityActivate();
};
