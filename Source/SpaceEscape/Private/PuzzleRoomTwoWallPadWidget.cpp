// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoWallPadWidget.h"

#include "GravityChange.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPuzzleRoomTwoWallPadWidget::NativeConstruct()
{
	Super::NativeConstruct();
	gc = Cast<AGravityChange>(UGameplayStatics::GetActorOfClass(this, AGravityChange::StaticClass()));
}

void UPuzzleRoomTwoWallPadWidget::GravityActivate()
{
	textBlock_Activate->SetText(FText::FromString("Activated"));
	image_Activate->SetBrushTintColor(FColor::Red);
	gc->ChangeGravity(true);
}
