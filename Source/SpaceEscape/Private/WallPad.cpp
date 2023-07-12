// Fill out your copyright notice in the Description page of Project Settings.


#include "WallPad.h"

#include "DoorManager.h"
#include "EscapePlayer.h"
#include "PuzzleRoomTwoWallPadWidget.h"
#include "RoomManager.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EmergencyProtocolWidget.h"

// Sets default values
AWallPad::AWallPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pad Mesh"));
	SetRootComponent(meshComp);
	meshComp->SetRelativeScale3D(FVector(0.2f));
	meshComp->SetSimulatePhysics(false);
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/sci-fi-tablet/source/Tablet/Tablet.Tablet'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	wallScreenComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Wall Screen Widget"));
	wallScreenComp->SetupAttachment(RootComponent);
	wallScreenComp->SetCollisionProfileName(TEXT("NoCollision"));

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetRelativeScale3D(FVector(0.15f));
	boxComp->SetBoxExtent(FVector(16.f, 32.f, 8.f));
	boxComp->SetRelativeLocation(FVector(-17.5f, 10.5f, 0));
	boxComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	ConstructorHelpers::FObjectFinder<USoundBase>tempSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/BatteryChargingSoundReverse.BatteryChargingSoundReverse'"));
    if (tempSound.Succeeded())
    {
		gravityOffSound = tempSound.Object;
    }

	Tags.Add(FName("Sense"));
}

// Called when the game starts or when spawned
void AWallPad::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AWallPad::OnOverlap);
	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));
	gm = Cast<ASpaceEscapeGameModeBase>(GetWorld()->GetAuthGameMode());

	// 인게임 인스턴스에서 wallScreenComp에 각각 수동으로 설정한 WBP에 따라 동작하는 기능이 다름
	// 겟위젯을 해와서 캐스팅이 되는지 여부로 어떤 위젯이 세팅되어있는지 판단됨
	gravityWidget = Cast<UPuzzleRoomTwoWallPadWidget>(wallScreenComp->GetWidget());
	protocolWidget = Cast<UEmergencyProtocolWidget>(wallScreenComp->GetWidget());
	if (protocolWidget)
	{
		protocolWidget->ChangeProtocolLanguage(gm->currentLanguageSetting);
		gm->changeLanguageDele.AddUFunction(protocolWidget, FName("ChangeProtocolLanguage"));
	}
}

// Called every frame
void AWallPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallPad::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto pl = Cast<AEscapePlayer>(OtherActor);
	if (gravityWidget && pl && !bActiveOnce)
	{
		gravityWidget->GravityActivate();
		bActiveOnce = true;
		ReportClear();

		UGameplayStatics::PlaySound2D(this, gravityOffSound);

		// 만약 문이 열려있다면 모든 문 닫히게 처리
		auto dm = Cast<ADoorManager>(UGameplayStatics::GetActorOfClass(this, ADoorManager::StaticClass()));
		if (dm && dm->bOpened)
		{
			dm->CloseAllDoors();
		}
	}
}

