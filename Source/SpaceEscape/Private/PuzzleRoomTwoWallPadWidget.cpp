// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoWallPadWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPuzzleRoomTwoWallPadWidget::GravityActivate()
{
	textBlock_Activate->SetText(FText::FromString("Activated"));
	image_Activate->SetBrushTintColor(FColor::Red);
}
