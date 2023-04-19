// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneTouchPad.h"

#include "EscapePlayer.h"
#include "PuzzleRoomOneTouchPadPanelWidget.h"
#include "Components/BoxComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Haptics/HapticFeedbackEffect_Base.h"

APuzzleRoomOneTouchPad::APuzzleRoomOneTouchPad()
{
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TouchPanel Mesh"));
	SetRootComponent(meshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/WallPanel/WallPanel_Cube.WallPanel_Cube'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetRelativeScale3D(FVector(0.1f));
	meshComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));

	screenWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Screen Widget"));
	screenWidgetComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FClassFinder<UUserWidget>tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_TouchPadPanel.WBP_TouchPadPanel_C'"));
	if (tempWidget.Succeeded())
	{
		screenWidgetComp->SetWidgetClass(tempWidget.Class);
	}
	screenWidgetComp->SetRelativeLocationAndRotation(FVector(0, 10.f, 30.f), FRotator(0, 90, 0));
	screenWidgetComp->SetRelativeScale3D(FVector(0.1f, 0.08f, 0.1f));
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

	panelWidgetClass = Cast<UPuzzleRoomOneTouchPadPanelWidget>(screenWidgetComp->GetWidget());
	if (panelWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Panel Widget Casting Failed!!"));
	}

	panelWidgetClass->deleteCurrentScreen();
}

// 터치패드의 입력에 따라 기능을 실행하는 함수
void APuzzleRoomOneTouchPad::TouchPadInput(FString number)
{
	if (number == "r")
	{
		// Enter 처리
		CheckPassword();
		panelWidgetClass->deleteCurrentScreen();
		return;
	}

	if (number == "e")
	{
		// Delete 처리
		panelWidgetClass->deleteCurrentScreen();
		return;
	}

	panelWidgetClass->addCurrentScreen(number);
}

// 터치패드에 오버랩되면 실행되는 함수
void APuzzleRoomOneTouchPad::TouchPadOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 터치 입력 후 일정 시간동안 입력 안되게 처리
	if (!bCanTouch)
	{
		return;
	}
	bCanTouch = false;
	FTimerHandle touchDelayHandle;
	GetWorldTimerManager().SetTimer(touchDelayHandle, FTimerDelegate::CreateLambda([&]()
		{
			bCanTouch = true;
		}), touchDelay, false);

	// 오버랩된 부분의 컴포넌트 이름의 마지막 글자를 가져와서 실행함수에 넘겨준다
	FString compName;
	compName.AppendChar(OverlappedComponent->GetName()[OverlappedComponent->GetName().Len() - 1]);
	TouchPadInput(compName);

	UE_LOG(LogTemp, Warning, TEXT("compName is : %s"), *compName);

	// 오버랩된 물체가 플레이어라면 오른손 진동울리게 처리
	auto player = Cast<AEscapePlayer>(OtherActor);
	if (player)
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
	}
}

// 엔터입력 들어왔을 때 패스워드 확인하는 함수
void APuzzleRoomOneTouchPad::CheckPassword()
{
	if (panelWidgetClass->GetCurrentScreen() == answer)
	{
		// 성공 처리
		ReportClear();
		UE_LOG(LogTemp, Warning, TEXT("Succeeded"));
	}
	else
	{
		// 실패 처리
		UE_LOG(LogTemp, Warning, TEXT("Failed"));
	}
}
