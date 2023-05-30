// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorse.h"

#include "PuzzleRoomThreeMorseButton.h"
#include "PuzzleRoomThreeMorseScreenWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"
#include "PuzzleRoomThreeMorseLever.h"

APuzzleRoomThreeMorse::APuzzleRoomThreeMorse()
{
	screenComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Screen Widget Comp"));
	SetRootComponent(screenComp);
	ConstructorHelpers::FClassFinder<UUserWidget>tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_MorseScreen.WBP_MorseScreen_C'"));
	if (tempWidget.Succeeded())
	{
		screenComp->SetWidgetClass(tempWidget.Class);
	}
	screenComp->SetDrawSize(FVector2D(1920.f, 1080.f));
}

void APuzzleRoomThreeMorse::BeginPlay()
{
	Super::BeginPlay();
	// 출력할 화면 캐싱
	screenWidget = Cast<UPuzzleRoomThreeMorseScreenWidget>(screenComp->GetWidget());

	// 모스 버튼 찾아서 캐싱
	for (TActorIterator<APuzzleRoomThreeMorseButton> it(GetWorld()); it; ++it)
	{
		APuzzleRoomThreeMorseButton* btn = *it;
		btn->morseButtonDele.BindUFunction(this, TEXT("addToTempString"));
	}

	// 모스 레버 찾아서 캐싱
	for (TActorIterator<APuzzleRoomThreeMorseLever> it(GetWorld()); it; ++it)
	{
		APuzzleRoomThreeMorseLever* lever = *it;
		lever->morseLeverDele.BindUFunction(this, TEXT("Enter"));
	}

	// 스크린 출력문자 초기화
	setScreenText(screenString);
}

// 모스버튼 신호를 받아서 임시문자열에 추가하는 함수
void APuzzleRoomThreeMorse::addToTempString(float second)
{
	if (second >= 0.6f)
	{
		tempString.AppendChar('1');
	}
	else
	{
		tempString.AppendChar('0');
	}
	//UE_LOG(LogTemp, Warning, TEXT("tempstring is : %s"), *tempString);
	//UE_LOG(LogTemp, Warning, TEXT("pushed time is : %f"), second);
}

// 임시 문자열을 번역해서 스크린에 출력할 문자열에 추가하는 함수
void APuzzleRoomThreeMorse::Enter()
{
	// 임시문자열이 비어있거나 맵에 없는 키라면 임시문자열비우고 리턴
	if (tempString.IsEmpty() || !morse.Contains(tempString))
	{
		tempString.Empty();
		return;
	}

	screenString.AppendChar(Translater(tempString));
	setScreenText(screenString);

	if (screenString.Len() == 4)
	{
		FTimerHandle checkHandle;
		GetWorldTimerManager().SetTimer(checkHandle, FTimerDelegate::CreateLambda([&]()
			{
				CheckRightOrWrong();
			}), 2.f, false);
	}

	tempString.Empty();
}

// 정답인지 틀렸는지 확인하는 함수
void APuzzleRoomThreeMorse::CheckRightOrWrong()
{
	if (screenString == "ARTH" && !bAnswerOnce)
	{
		bAnswerOnce = true;
		screenWidget->TextBlock_E->SetVisibility(ESlateVisibility::Hidden);
		setScreenText("END");
		ReportClear();
	}
	else
	{
		setScreenText("");
	}

	screenString.Empty();
}

// 스크린에 인자로 들어온 String값을 출력하는 함수
void APuzzleRoomThreeMorse::setScreenText(FString string)
{
	screenWidget->TextBlock_Morse->SetText(FText::FromString(string));
}

// 현재 입력중인 screenstring을 반환하는 함수
FString APuzzleRoomThreeMorse::GetScreenString()
{
	return screenString;
}

// 현재 입력중인 screenstring을 비우는 함수
void APuzzleRoomThreeMorse::EmptyScreenString()
{
	screenString.Empty();
}

// 0와 1로 이뤄진 임시문자열을 영문자로 번역해서 뱉어내는 함수
char APuzzleRoomThreeMorse::Translater(FString code)
{
	return morse[code];
}
