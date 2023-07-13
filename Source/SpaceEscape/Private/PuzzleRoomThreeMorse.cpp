// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorse.h"

#include "PuzzleRoomThreeMorseButton.h"
#include "PuzzleRoomThreeMorseScreenWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"
#include "PuzzleRoomThreeMorseLever.h"
#include "RoomManager.h"
#include "SpaceEscapeGameModeBase.h"
#include "SpaceShip.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

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

	// 게임 종료후 이니셜 입력 기능위해 룸매니저 캐싱
	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));
	rm->endingDele.AddUFunction(this, FName("ForEndingRanking"));

	// 언어 설정을 위한 게임모드 캐싱
	gm = Cast<ASpaceEscapeGameModeBase>(GetWorld()->GetAuthGameMode());

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
	setScreenText("");
	if (gm->currentLanguageSetting == ELanguageSettings::KOREAN)
	{
		//한글인 경우
		screenWidget->switcher->SetActiveWidgetIndex(4);
	}
	else if (gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
	{
		//영어인 경우
		screenWidget->switcher->SetActiveWidgetIndex(8);
	}
	

	// 조이스틱 장착여부 전달받기 위해 우주선 캐싱
	ship = Cast<ASpaceShip>(UGameplayStatics::GetActorOfClass(this, ASpaceShip::StaticClass()));
	ship->spaceShipStickDele.AddUFunction(this, FName("WhenShipSticked"));
}

// 모스버튼 신호를 받아서 임시문자열에 추가하는 함수
void APuzzleRoomThreeMorse::addToTempString(const float second)
{
	// 조이스틱이 장착되지 않으면 동작하지 않게 처리
	if (!bIsSticked)
	{
		return;
	}

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
	// 조이스틱이 장착되지 않으면 동작하지 않게 처리
	if (!bIsSticked)
	{
		return;
	}

	// 안내문 지우고
	screenWidget->switcher->SetActiveWidgetIndex(0);

	// 임시문자열이 비어있거나 맵에 없는 키라면 임시문자열비우고 리턴
	if (tempString.IsEmpty() || !morse.Contains(tempString))
	{
		tempString.Empty();
		return;
	}

	screenString.AppendChar(Translater(tempString));
	setScreenText(screenString);

	if (screenString.Len() == 5)
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
	if (screenString == morseAnswer && !bAnswerOnce)
	{
		// 정답일 경우
		bAnswerOnce = true;
		if (gm->currentLanguageSetting == ELanguageSettings::KOREAN)
		{
			//한글인 경우
			screenWidget->switcher->SetActiveWidgetIndex(2);
		}
		else if (gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
		{
			//영어인 경우
			screenWidget->switcher->SetActiveWidgetIndex(6);
		}
		ReportClear();
	}

	if (!bAnswerOnce)
	{
		// 틀렸을 경우
		if (gm->currentLanguageSetting == ELanguageSettings::KOREAN)
		{
			//한글인 경우
			screenWidget->switcher->SetActiveWidgetIndex(3);
		}
		else if (gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
		{
			//영어인 경우
			screenWidget->switcher->SetActiveWidgetIndex(7);
		}
		
	}

	// 풀고나서 이름 입력할 때 알아서 비워지게
	screenString.Empty();
	setScreenText(screenString);
}

// 스크린에 인자로 들어온 String값을 출력하는 함수
void APuzzleRoomThreeMorse::setScreenText(const FString& string)
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
char APuzzleRoomThreeMorse::Translater(const FString& code)
{
	return morse[code];
}

// 랭킹 입력위해 출력창 비우는 함수
void APuzzleRoomThreeMorse::ForEndingRanking()
{
	screenString.Empty();
	setScreenText(screenString);
}

void APuzzleRoomThreeMorse::WhenShipSticked()
{
	bIsSticked = true;
	if(gm->currentLanguageSetting == ELanguageSettings::KOREAN)
	{
		//한글인 경우
		screenWidget->switcher->SetActiveWidgetIndex(1);
	}
	else if(gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
	{
		//영어인 경우
		screenWidget->switcher->SetActiveWidgetIndex(5);
	}
}
