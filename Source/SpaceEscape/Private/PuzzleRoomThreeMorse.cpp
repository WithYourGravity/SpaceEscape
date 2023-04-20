// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorse.h"

#include "PuzzleRoomThreeMorseButton.h"
#include "PuzzleRoomThreeMorseScreenWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"

APuzzleRoomThreeMorse::APuzzleRoomThreeMorse()
{
	screenComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Screen Widget Comp"));
	SetRootComponent(screenComp);
	ConstructorHelpers::FClassFinder<UUserWidget>tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_MorseScreen.WBP_MorseScreen_C'"));
	if (tempWidget.Succeeded())
	{
		screenComp->SetWidgetClass(tempWidget.Class);
	}
}

void APuzzleRoomThreeMorse::BeginPlay()
{
	Super::BeginPlay();
	screenWidget = Cast<UPuzzleRoomThreeMorseScreenWidget>(screenComp->GetWidget());

	// 모스 버튼 찾아서 캐싱
	for (TActorIterator<APuzzleRoomThreeMorseButton> it(GetWorld()); it; ++it)
	{
		APuzzleRoomThreeMorseButton* btn = *it;
		btn->morseButtonDele.BindUFunction(this, TEXT("addToTempString"));
	}
}

void APuzzleRoomThreeMorse::addToTempString(float second)
{
	if (second >= 1)
	{
		tempString.AppendChar('1');
	}
	else
	{
		tempString.AppendChar('0');
	}
	UE_LOG(LogTemp, Warning, TEXT("tempstring is : %s"), *tempString);
}

void APuzzleRoomThreeMorse::Enter()
{
	screenWidget->TextBlock_Morse->SetText(FText::FromString(Translater(tempString)));
}

FString APuzzleRoomThreeMorse::Translater(FString code)
{
	//임시
	return code;
}
