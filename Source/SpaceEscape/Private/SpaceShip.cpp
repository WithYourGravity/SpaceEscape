// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "PuzzleRoomThreeJoystick.h"
#include "RoomManager.h"
#include "SpaceShipJoystick.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	shipMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("shipMeshComp"));
	SetRootComponent(shipMeshComp);
	shipMeshComp->SetCollisionProfileName(FName("BlockAllDynamic"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_SpaceShip.SM_SpaceShip'"));
    if (tempMesh.Succeeded())
    {
		shipMeshComp->SetStaticMesh(tempMesh.Object);
    }

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetBoxExtent(FVector(600.f, 400.f, 300.f));
	boxComp->SetRelativeLocation(FVector(300.f, 200.f, 200.f));
	boxComp->SetRelativeRotation(FRotator(0, -30.f, 0));

	forLocComp = CreateDefaultSubobject<USphereComponent>(TEXT("forLocComp"));
	forLocComp->SetupAttachment(RootComponent);
	forLocComp->SetCollisionProfileName(FName("NoCollision"));
	forLocComp->SetRelativeLocation(FVector(280.f, 0, 220.f));
	forLocComp->SetSphereRadius(16.f);

	forJoyLocComp = CreateDefaultSubobject<USphereComponent>(TEXT("forJoyLocComp"));
	forJoyLocComp->SetupAttachment(RootComponent);
	forJoyLocComp->SetCollisionProfileName(FName("OverlapAllDynamic"));
	forJoyLocComp->SetRelativeLocation(FVector(323.f, 0, 208.f));
	forJoyLocComp->SetSphereRadius(8.f);

	morseCodeTableComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("morseCodeTableComp"));
	morseCodeTableComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMorseCode(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/PlaneMorse.PlaneMorse'"));
    if (tempMorseCode.Succeeded())
    {
		morseCodeTableComp->SetStaticMesh(tempMorseCode.Object);
    }
	morseCodeTableComp->SetRelativeLocationAndRotation(FVector(313.97f, 47.36f, 226.31f), FRotator(13.39f, 172.f, 89.f));
	morseCodeTableComp->SetRelativeScale3D(FVector(0.12f, 0.094f, 0.05f));
	morseCodeTableComp->SetCollisionProfileName(FName("NoCollision"));

	morseScreenActorComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("morseScreenActorComp"));
	morseScreenActorComp->SetupAttachment(RootComponent);
	morseScreenActorComp->SetRelativeLocationAndRotation(FVector(353.54f, 21.28f, 216.f), FRotator(43.65f, 180.f, 0));
	morseScreenActorComp->SetRelativeScale3D(FVector(0.16f));
	ConstructorHelpers::FClassFinder<AActor>tempMorseBP(TEXT("/Script/Engine.Blueprint'/Game/LTG/Blueprints/BP_PuzzleRoomThreeMorse.BP_PuzzleRoomThreeMorse_C'"));
    if (tempMorseBP.Succeeded())
    {
		morseScreenActorComp->SetChildActorClass(tempMorseBP.Class);
    }

	morseLeverActorComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("morseLeverActorComp"));
	morseLeverActorComp->SetupAttachment(RootComponent);
	morseLeverActorComp->SetRelativeLocationAndRotation(FVector(341.2f, -0.02f, 213.11f), FRotator(0, 90.f, -45.f));
	ConstructorHelpers::FClassFinder<AActor>tempLeverBP(TEXT("/Script/Engine.Blueprint'/Game/LTG/Blueprints/BP_PuzzleRoomThreeMorseLever.BP_PuzzleRoomThreeMorseLever_C'"));
	if (tempLeverBP.Succeeded())
	{
		morseLeverActorComp->SetChildActorClass(tempLeverBP.Class);
	}

	morseButtonActorComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("morseButtonActorComp"));
	morseButtonActorComp->SetupAttachment(RootComponent);
	morseButtonActorComp->SetRelativeScale3D(FVector(1.1f));
	morseButtonActorComp->SetRelativeLocationAndRotation(FVector(340.86f, -25.47f, 211.42f), FRotator(0, 90.f, 40.f));
	ConstructorHelpers::FClassFinder<AActor>tempButtonBP(TEXT("/Script/Engine.Blueprint'/Game/LTG/Blueprints/BP_PuzzleRoomThreeMorseButton.BP_PuzzleRoomThreeMorseButton_C'"));
	if (tempButtonBP.Succeeded())
	{
		morseButtonActorComp->SetChildActorClass(tempButtonBP.Class);
	}

	rankingActorComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("rankingActorComp"));
	rankingActorComp->SetupAttachment(RootComponent);
	rankingActorComp->SetRelativeLocationAndRotation(FVector(2000.f, 0, 800.f), FRotator(0, 180.f, 0));
	ConstructorHelpers::FClassFinder<AActor>tempRanking(TEXT("/Script/Engine.Blueprint'/Game/LTG/Blueprints/BP_Ranking.BP_Ranking_C'"));
	if (tempRanking.Succeeded())
	{
		rankingActorComp->SetChildActorClass(tempRanking.Class);
	}

	billboardComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("billboardComp"));
	billboardComp->SetupAttachment(boxComp);
	billboardComp->SetRelativeLocation(FVector(40, -50, -100));
	billboardComp->SetVisibility(false);
	billboardComp->SetHiddenInGame(false);
	ConstructorHelpers::FObjectFinder<UTexture2D>tempBillIMG(TEXT("/Script/Engine.Texture2D'/Game/Yeni/Images/OnBoarding1.OnBoarding1'"));
    if (tempBillIMG.Succeeded())
    {
		billboardComp->SetSprite(tempBillIMG.Object);
    }
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ASpaceShip::OnOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &ASpaceShip::EndOverlap);
	forJoyLocComp->OnComponentBeginOverlap.AddDynamic(this, &ASpaceShip::OnOverlapJoystick);

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));

	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));
	rm->gameClearDele.AddUFunction(this, FName("SpawnControlableJoystick"));
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpaceShip::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 버튼허용 처리
	auto pl = Cast<AEscapePlayer>(OtherActor);
	if (pl)
	{
		bReadyToBoarding = true;

		//빌보드 켜기
		billboardComp->SetVisibility(true);
	}
}

void ASpaceShip::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 허용 취소 처리
	auto pl = Cast<AEscapePlayer>(OtherActor);
	if (pl)
	{
		bReadyToBoarding = false;

		// 빌보드 끄기
		billboardComp->SetVisibility(false);
	}
}

// 길찾기 퍼즐의 조이스틱이 우주선에 조종석에 오버랩되면 실행되는 함수
void ASpaceShip::OnOverlapJoystick(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto stick = Cast<APuzzleRoomThreeJoystick>(OtherActor);
	if (stick)
	{
		stickComp = stick->stickMeshComp;
		stick->grabComp->TryRelease();
		stickComp->SetWorldRotation(GetActorRotation());
		stickComp->SetSimulatePhysics(false);
		stickComp->SetCollisionProfileName(FName("NoCollision"));

		stickLoc = forJoyLocComp->GetComponentLocation();
		stickFinalLoc = stickLoc + FVector(0, 0, -10.f);
		stickComp->SetWorldLocation(stickLoc);
		GetWorldTimerManager().SetTimer(stickDownHandle, FTimerDelegate::CreateLambda([&]()
			{
				stickComp->SetWorldLocation(FMath::Lerp(stickLoc, stickFinalLoc, stickTimer));
				stickTimer += GetWorld()->GetDeltaSeconds() * 0.2f;
				stickTimer = FMath::Clamp(stickTimer, 0.f, 1.f);
				if (stickTimer == 1)
				{
					GetWorldTimerManager().ClearTimer(stickDownHandle);
				}
			}), GetWorld()->GetDeltaSeconds(), true, 1.f);
	}
}

// 우주선에 탑승시키는 함수
void ASpaceShip::BoardingShip()
{
	// 비행기 탑승범위가 아니라면 리턴
	if (!bReadyToBoarding && !bIsBoarding)
	{
		return;
	}

	// 이미 타고 있다면
	if (bIsBoarding)
	{
		// 플레이어 이동 켜고
		if (bPlayerUseTeleport)
		{
			player->moveMode = EMoveModeState::TELEPORT;
		}
		player->GetCharacterMovement()->MaxWalkSpeed = 400;

		// 바깥으로 이동시킨다
		player->SetActorLocation(GetActorLocation() + FVector(0, -400.f, 100.f));
		player->Turn(FVector2D(-50, 0));
		bIsBoarding = false;
		billboardComp->SetVisibility(true);
	}
	else
	{
		// 플레이어 이동끄고 
		player->GetCharacterMovement()->MaxWalkSpeed = 0;
		if (player->moveMode == EMoveModeState::TELEPORT)
		{
			bPlayerUseTeleport = true;
		}
		player->moveMode = EMoveModeState::JOYSTICK;

		// 조종석 위치로 이동시킨다
		player->SetActorLocation(forLocComp->GetComponentLocation());
		player->Turn(FVector2D(50, 0));
		bIsBoarding = true;
		billboardComp->SetVisibility(false);
	}
}

// 모스까지 완료시 우주선 돌릴수있는 조이스틱 스폰하는 함수
void ASpaceShip::SpawnControlableJoystick()
{
	if (stickComp)
	{
		stickComp->DestroyComponent();
	}

	player->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	stickLoc = forJoyLocComp->GetComponentLocation() + FVector(0, 0, -16.5f);
	controlableStick = GetWorld()->SpawnActor<ASpaceShipJoystick>(ASpaceShipJoystick::StaticClass(), stickLoc, GetActorRotation().Add(0, -90.f, 0));
	controlableStick->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	stickFinalLoc = stickLoc + FVector(0, 0, 10.f);
	stickTimer = 0;
	GetWorldTimerManager().SetTimer(stickDownHandle, FTimerDelegate::CreateLambda([&]()
		{
			controlableStick->SetActorLocation(FMath::Lerp(stickLoc, stickFinalLoc, stickTimer));
			stickTimer += GetWorld()->GetDeltaSeconds() * 0.5f;
			stickTimer = FMath::Clamp(stickTimer, 0.f, 1.f);
			if (stickTimer == 1)
			{
				GetWorldTimerManager().ClearTimer(stickDownHandle);
			}
		}), GetWorld()->GetDeltaSeconds(), true, 1.f);
}

