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
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	class UTextBlock* textBlock_Activate;
	UPROPERTY(EditDefaultsOnly)
	class UImage* image_Activate;

	void GravityActivate();

private:
	UPROPERTY()
	class AGravityChange* gc;
};
