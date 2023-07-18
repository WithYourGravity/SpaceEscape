// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorse.h"

#include "PuzzleRoomThreeMorseButton.h"
#include "PuzzleRoomThreeMorseScreenWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "components/BoxComponent.h"
#include "EngineUtils.h"
#include "PuzzleRoomThreeMorseLever.h"
#include "RoomManager.h"
#include "SpaceEscapeGameModeBase.h"
#include "SpaceShip.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "EscapePlayer.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"

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

	// ȭ�� ��ġ�ϸ� ���� �������� �� �ڽ��ݸ��� ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetRelativeLocation(FVector(0, 650, 400));
	boxComp->SetBoxExtent(FVector(64, 256, 128));
	boxComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	// ���� ����
	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base>tempHaptic(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/LTG/UI/HF_TouchFeedback.HF_TouchFeedback'"));
	if (tempHaptic.Succeeded())
	{
		hapticFeedback = tempHaptic.Object;
	}
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
	gm->changeLanguageDele.AddUFunction(this, FName("ChangeLanguage"));
	// �� ��ư ã�Ƽ� ĳ��
	for (TActorIterator<APuzzleRoomThreeMorseButton> it(GetWorld()); it; ++it)
	{
		APuzzleRoomThreeMorseButton* btn = *it;
		btn->morseButtonDele.BindUFunction(this, TEXT("addToTempString"));
	}

	//Setting â���� �� �������� �ʾ��� ��� Spaceship�� Morse Screen�� ��Ÿ�� ��� ���ϱ�
	if (gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
	{
		//������ ���
		screenWidget->switcher->SetActiveWidgetIndex(8);
	}
	else if (gm->currentLanguageSetting == ELanguageSettings::KOREAN)
	{
		//�ѱ��� ���
		screenWidget->switcher->SetActiveWidgetIndex(4);
	}

	// �� ���� ã�Ƽ� ĳ��
	for (TActorIterator<APuzzleRoomThreeMorseLever> it(GetWorld()); it; ++it)
	{
		APuzzleRoomThreeMorseLever* lever = *it;
		lever->morseLeverDele.BindUFunction(this, TEXT("Enter"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("Ship Language : %d"), gm->currentLanguageSetting)

	// ��ũ�� ��¹��� �ʱ�ȭ
	setScreenText("");
	//ChangeLanguage(gm->currentLanguageSetting);

	// ���̽�ƽ �������� ���޹ޱ� ���� ���ּ� ĳ��
	ship = Cast<ASpaceShip>(UGameplayStatics::GetActorOfClass(this, ASpaceShip::StaticClass()));
	ship->spaceShipStickDele.AddUFunction(this, FName("WhenShipSticked"));

	// ȭ�� ��ġ�� �Է��� ���� ����� �Լ��� ���ε�
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomThreeMorse::OnOverlap);
}

void APuzzleRoomThreeMorse::ChangeLanguage(ELanguageSettings language)
{
	switch (language)
	{
	case  ELanguageSettings::ENGLISH:
		//������ ���
		screenWidget->switcher->SetActiveWidgetIndex(8);
		//UE_LOG(LogTemp, Warning, TEXT("First screen of Ship English: %d"), gm->currentLanguageSetting)
		break;
	case  ELanguageSettings::KOREAN:
		//�ѱ��� ���
		screenWidget->switcher->SetActiveWidgetIndex(4);
		//UE_LOG(LogTemp, Warning, TEXT("First screen of Ship : %d"), gm->currentLanguageSetting)
	break;
	}

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

// ���ּ��� ��Ʈ�ѷ��� �������� �� ����Ǵ� �Լ�
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

// ȭ�� ��ġ�� �Է��� �� ����� �Լ�
void APuzzleRoomThreeMorse::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto pl = Cast<AEscapePlayer>(OtherActor);
	if (pl)
	{
		// ȭ���� ���ڿ� ����
		screenString.Empty();
		setScreenText(screenString);
		// �Է��ϴ� �𽺹��ڿ��� ���
		tempString.Empty();

		// �հ� �������Ǹ� ���� �︮�� ó��
		if (OtherComp->GetName().Contains("left"))
		{
			pl->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
		}
		else
		{
			pl->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
		}
	}
}

