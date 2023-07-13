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
	// ����� ȭ�� ĳ��
	screenWidget = Cast<UPuzzleRoomThreeMorseScreenWidget>(screenComp->GetWidget());

	// ���� ������ �̴ϼ� �Է� ������� ��Ŵ��� ĳ��
	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));
	rm->endingDele.AddUFunction(this, FName("ForEndingRanking"));

	// ��� ������ ���� ���Ӹ�� ĳ��
	gm = Cast<ASpaceEscapeGameModeBase>(GetWorld()->GetAuthGameMode());

	// �� ��ư ã�Ƽ� ĳ��
	for (TActorIterator<APuzzleRoomThreeMorseButton> it(GetWorld()); it; ++it)
	{
		APuzzleRoomThreeMorseButton* btn = *it;
		btn->morseButtonDele.BindUFunction(this, TEXT("addToTempString"));
	}

	// �� ���� ã�Ƽ� ĳ��
	for (TActorIterator<APuzzleRoomThreeMorseLever> it(GetWorld()); it; ++it)
	{
		APuzzleRoomThreeMorseLever* lever = *it;
		lever->morseLeverDele.BindUFunction(this, TEXT("Enter"));
	}

	// ��ũ�� ��¹��� �ʱ�ȭ
	setScreenText("");
	if (gm->currentLanguageSetting == ELanguageSettings::KOREAN)
	{
		//�ѱ��� ���
		screenWidget->switcher->SetActiveWidgetIndex(4);
	}
	else if (gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
	{
		//������ ���
		screenWidget->switcher->SetActiveWidgetIndex(8);
	}
	

	// ���̽�ƽ �������� ���޹ޱ� ���� ���ּ� ĳ��
	ship = Cast<ASpaceShip>(UGameplayStatics::GetActorOfClass(this, ASpaceShip::StaticClass()));
	ship->spaceShipStickDele.AddUFunction(this, FName("WhenShipSticked"));
}

// �𽺹�ư ��ȣ�� �޾Ƽ� �ӽù��ڿ��� �߰��ϴ� �Լ�
void APuzzleRoomThreeMorse::addToTempString(const float second)
{
	// ���̽�ƽ�� �������� ������ �������� �ʰ� ó��
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

// �ӽ� ���ڿ��� �����ؼ� ��ũ���� ����� ���ڿ��� �߰��ϴ� �Լ�
void APuzzleRoomThreeMorse::Enter()
{
	// ���̽�ƽ�� �������� ������ �������� �ʰ� ó��
	if (!bIsSticked)
	{
		return;
	}

	// �ȳ��� �����
	screenWidget->switcher->SetActiveWidgetIndex(0);

	// �ӽù��ڿ��� ����ְų� �ʿ� ���� Ű��� �ӽù��ڿ����� ����
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

// �������� Ʋ�ȴ��� Ȯ���ϴ� �Լ�
void APuzzleRoomThreeMorse::CheckRightOrWrong()
{
	if (screenString == morseAnswer && !bAnswerOnce)
	{
		// ������ ���
		bAnswerOnce = true;
		if (gm->currentLanguageSetting == ELanguageSettings::KOREAN)
		{
			//�ѱ��� ���
			screenWidget->switcher->SetActiveWidgetIndex(2);
		}
		else if (gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
		{
			//������ ���
			screenWidget->switcher->SetActiveWidgetIndex(6);
		}
		ReportClear();
	}

	if (!bAnswerOnce)
	{
		// Ʋ���� ���
		if (gm->currentLanguageSetting == ELanguageSettings::KOREAN)
		{
			//�ѱ��� ���
			screenWidget->switcher->SetActiveWidgetIndex(3);
		}
		else if (gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
		{
			//������ ���
			screenWidget->switcher->SetActiveWidgetIndex(7);
		}
		
	}

	// Ǯ���� �̸� �Է��� �� �˾Ƽ� �������
	screenString.Empty();
	setScreenText(screenString);
}

// ��ũ���� ���ڷ� ���� String���� ����ϴ� �Լ�
void APuzzleRoomThreeMorse::setScreenText(const FString& string)
{
	screenWidget->TextBlock_Morse->SetText(FText::FromString(string));
}

// ���� �Է����� screenstring�� ��ȯ�ϴ� �Լ�
FString APuzzleRoomThreeMorse::GetScreenString()
{
	return screenString;
}

// ���� �Է����� screenstring�� ���� �Լ�
void APuzzleRoomThreeMorse::EmptyScreenString()
{
	screenString.Empty();
}

// 0�� 1�� �̷��� �ӽù��ڿ��� �����ڷ� �����ؼ� ���� �Լ�
char APuzzleRoomThreeMorse::Translater(const FString& code)
{
	return morse[code];
}

// ��ŷ �Է����� ���â ���� �Լ�
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
		//�ѱ��� ���
		screenWidget->switcher->SetActiveWidgetIndex(1);
	}
	else if(gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
	{
		//������ ���
		screenWidget->switcher->SetActiveWidgetIndex(5);
	}
}
