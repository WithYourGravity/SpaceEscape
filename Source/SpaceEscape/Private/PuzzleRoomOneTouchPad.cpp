// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneTouchPad.h"

#include "EscapePlayer.h"
#include "GravityChange.h"
#include "PuzzleRoomOneTouchPadPanelWidget.h"
#include "PuzzleRoomTwoTouchPadPanelWidget.h"
#include "Components/BoxComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"
#include "Kismet/GameplayStatics.h"


APuzzleRoomOneTouchPad::APuzzleRoomOneTouchPad()
{
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TouchPanel Mesh"));
	SetRootComponent(meshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/WallPanel/WallPanel_Cube.WallPanel_Cube'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetRelativeScale3D(FVector(0.4f));
	meshComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));

	screenWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Screen Widget"));
	screenWidgetComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FClassFinder<UUserWidget>tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_TouchPadPanel.WBP_TouchPadPanel_C'"));
	if (tempWidget.Succeeded())
	{
		screenWidgetComp->SetWidgetClass(tempWidget.Class);
	}
	screenWidgetComp->SetRelativeLocationAndRotation(FVector(0, 10.f, 30.f), FRotator(0, 90, 0));
	screenWidgetComp->SetRelativeScale3D(FVector(1.f, 0.08f, 0.095f));
	screenWidgetComp->SetDrawSize(FVector2D(1920, 1080));
	screenWidgetComp->SetCollisionProfileName(TEXT("NoCollision"));

	touchKeyComp0 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey0"));
	touchKeyComp0->SetupAttachment(RootComponent);
	touchKeyComp0->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp0->SetRelativeLocation(FVector(-38.f, 14.f, -85.f));
	touchKeyComp0->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyComp1 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey1"));
	touchKeyComp1->SetupAttachment(RootComponent);
	touchKeyComp1->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp1->SetRelativeLocation(FVector(-60.f, 14.f, -42.f));
	touchKeyComp1->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyComp2 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey2"));
	touchKeyComp2->SetupAttachment(RootComponent);
	touchKeyComp2->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp2->SetRelativeLocation(FVector(-38.f, 14.f, -42.f));
	touchKeyComp2->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyComp3 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey3"));
	touchKeyComp3->SetupAttachment(RootComponent);
	touchKeyComp3->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp3->SetRelativeLocation(FVector(-16.f, 14.f, -42.f));
	touchKeyComp3->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyComp4 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey4"));
	touchKeyComp4->SetupAttachment(RootComponent);
	touchKeyComp4->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp4->SetRelativeLocation(FVector(-60.f, 14.f, -56.f));
	touchKeyComp4->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyComp5 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey5"));
	touchKeyComp5->SetupAttachment(RootComponent);
	touchKeyComp5->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp5->SetRelativeLocation(FVector(-38.f, 14.f, -56.f));
	touchKeyComp5->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyComp6 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey6"));
	touchKeyComp6->SetupAttachment(RootComponent);
	touchKeyComp6->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp6->SetRelativeLocation(FVector(-16.f, 14.f, -56.f));
	touchKeyComp6->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyComp7 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey7"));
	touchKeyComp7->SetupAttachment(RootComponent);
	touchKeyComp7->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp7->SetRelativeLocation(FVector(-60.f, 14.f, -70.f));
	touchKeyComp7->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyComp8 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey8"));
	touchKeyComp8->SetupAttachment(RootComponent);
	touchKeyComp8->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp8->SetRelativeLocation(FVector(-38.f, 14.f, -70.f));
	touchKeyComp8->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyComp9 = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKey9"));
	touchKeyComp9->SetupAttachment(RootComponent);
	touchKeyComp9->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyComp9->SetRelativeLocation(FVector(-16.f, 14.f, -70.f));
	touchKeyComp9->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyCompEnt = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKeyEnter"));
	touchKeyCompEnt->SetupAttachment(RootComponent);
	touchKeyCompEnt->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyCompEnt->SetRelativeLocation(FVector(-16.f, 14.f, -85.f));
	touchKeyCompEnt->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	touchKeyCompDel = CreateDefaultSubobject<UBoxComponent>(TEXT("touchKeyDelete"));
	touchKeyCompDel->SetupAttachment(RootComponent);
	touchKeyCompDel->SetRelativeScale3D(FVector(0.2f, 0.1f, 0.15f));
	touchKeyCompDel->SetRelativeLocation(FVector(-60.f, 14.f, -85.f));
	touchKeyCompDel->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));

	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base>tempHaptic(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/LTG/UI/HF_TouchFeedback.HF_TouchFeedback'"));
	if (tempHaptic.Succeeded())
	{
		hapticFeedback = tempHaptic.Object;
	}

	Tags.Add(FName("Sense"));
}

void APuzzleRoomOneTouchPad::BeginPlay()
{
	Super::BeginPlay();
	touchKeyComp0->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyComp1->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyComp2->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyComp3->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyComp4->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyComp5->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyComp6->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyComp7->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyComp8->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyComp9->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyCompEnt->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);
	touchKeyCompDel->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneTouchPad::TouchPadOverlap);

	// ���õ� Widget Blueprint�� ���� �����ϰ� if������ ����
	rmOnePanelWidget = Cast<UPuzzleRoomOneTouchPadPanelWidget>(screenWidgetComp->GetWidget());
	if (rmOnePanelWidget)
	{
		rmOnePanelWidget->deleteCurrentScreen();
		answer = "0714";
	}
	else
	{
		rmTwoPanelWidget = Cast<UPuzzleRoomTwoTouchPadPanelWidget>(screenWidgetComp->GetWidget());
		rmTwoPanelWidget->deleteCurrentScreen();
		answer = "4514";
	}

	// �߷� �Ŵ��� ĳ����
	gc = Cast<AGravityChange>(UGameplayStatics::GetActorOfClass(this, AGravityChange::StaticClass()));
}

// ��ġ�е��� �Է¿� ���� ����� �����ϴ� �Լ�
void APuzzleRoomOneTouchPad::TouchPadInput(FString number)
{
	if (number == "r")
	{
		// Enter ó��
		CheckPassword();
		if (rmOnePanelWidget)
		{
			rmOnePanelWidget->deleteCurrentScreen();
		}
		else
		{
			rmTwoPanelWidget->deleteCurrentScreen();
		}

		return;
	}

	if (number == "e")
	{
		// Delete ó��
		if (rmOnePanelWidget)
		{
			rmOnePanelWidget->deleteCurrentScreen();
		}
		else
		{
			rmTwoPanelWidget->deleteCurrentScreen();
		}
		return;
	}

	if (rmOnePanelWidget)
	{
		rmOnePanelWidget->addCurrentScreen(number);
	}
	else
	{
		rmTwoPanelWidget->addCurrentScreen(number);
	}
}

// ��ġ�е忡 �������Ǹ� ����Ǵ� �Լ�
void APuzzleRoomOneTouchPad::TouchPadOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ��ġ �Է� �� ���� �ð����� �Է� �ȵǰ� ó��
	// && ������ Ŭ����Ǹ� �Է� �ȵǰ� ó��
	if (!bCanTouch || GetbWasReport())
	{
		return;
	}
	bCanTouch = false;
	FTimerHandle touchDelayHandle;
	GetWorldTimerManager().SetTimer(touchDelayHandle, FTimerDelegate::CreateLambda([&]()
		{
			bCanTouch = true;
		}), touchDelay, false);

	// �������� �κ��� ������Ʈ �̸��� ������ ���ڸ� �����ͼ� �����Լ��� �Ѱ��ش�
	FString compName;
	compName.AppendChar(OverlappedComponent->GetName()[OverlappedComponent->GetName().Len() - 1]);
	TouchPadInput(compName);

	UE_LOG(LogTemp, Warning, TEXT("compName is : %s"), *compName);

	// �������� ��ü�� �÷��̾��� �����︮�� ó��
	auto player = Cast<AEscapePlayer>(OtherActor);
	if (player && OtherComp->GetName().Contains("left"))
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
	}
	else
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
	}
}

// �����Է� ������ �� �н����� Ȯ���ϴ� �Լ�
void APuzzleRoomOneTouchPad::CheckPassword()
{
	if (rmOnePanelWidget)
	{
		if (rmOnePanelWidget->GetCurrentScreen() == answer)
		{
			// ���� ó��
			ReportClear();
			rmOnePanelWidget->TextBlock_word->SetText(FText::FromString("SUCCESS"));
		}
		else
		{
			// ���� ó��
			rmOnePanelWidget->TextBlock_word->SetText(FText::FromString("FAILED"));
			FTimerHandle hd;
			GetWorldTimerManager().SetTimer(hd, FTimerDelegate::CreateLambda([&]()
				{
					rmOnePanelWidget->TextBlock_word->SetText(FText::FromString("ENTER PIN"));
				}), 1.f, false);
		}
	}
	else
	{
		if (rmTwoPanelWidget->GetCurrentScreen() == answer)
		{
			ReportClear();
			gc->ChangeGravity(false);
			rmTwoPanelWidget->TextBlock_word->SetText(FText::FromString("SUCCESS"));
		}
		else
		{
			rmTwoPanelWidget->TextBlock_word->SetText(FText::FromString("FAILED"));
			FTimerHandle hd;
			GetWorldTimerManager().SetTimer(hd, FTimerDelegate::CreateLambda([&]()
				{
					rmTwoPanelWidget->TextBlock_word->SetText(FText::FromString("ENTER PIN"));
				}), 1.f, false);
		}
	}
}
