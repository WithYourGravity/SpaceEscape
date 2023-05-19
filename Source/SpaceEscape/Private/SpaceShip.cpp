// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "PuzzleRoomThreeJoystick.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

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
	boxComp->SetBoxExtent(FVector(600.f, 300.f, 300.f));
	boxComp->SetRelativeLocation(FVector(300.f, 100.f, 200.f));
	boxComp->SetRelativeRotation(FRotator(0, -30.f, 0));

	forLocComp = CreateDefaultSubobject<USphereComponent>(TEXT("forLocComp"));
	forLocComp->SetupAttachment(RootComponent);
	forLocComp->SetCollisionProfileName(FName("NoCollision"));
	forLocComp->SetRelativeLocation(FVector(280.f, 0, 200.f));
	forLocComp->SetSphereRadius(72.f);

	forJoyLocComp = CreateDefaultSubobject<USphereComponent>(TEXT("forJoyLocComp"));
	forJoyLocComp->SetupAttachment(RootComponent);
	forJoyLocComp->SetCollisionProfileName(FName("NoCollision"));
	forJoyLocComp->SetRelativeLocation(FVector(323.f, 0, 208.f));
	forJoyLocComp->SetSphereRadius(16.f);
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ASpaceShip::OnOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &ASpaceShip::EndOverlap);
	forJoyLocComp->OnComponentBeginOverlap.AddDynamic(this, &ASpaceShip::OnOverlapJoystick);

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
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
	}
}

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
		stickFinalLoc = stickLoc + FVector(0, 0, -11.f);
		stickComp->SetWorldLocation(stickLoc);
		GetWorldTimerManager().SetTimer(stickDownHandle, FTimerDelegate::CreateLambda([&]()
			{
				stickComp->SetWorldLocation(FMath::Lerp(stickLoc, stickFinalLoc, stickTimer));
				stickTimer += GetWorld()->GetDeltaSeconds() * 0.4f;
				stickTimer = FMath::Clamp(stickTimer, 0.f, 1.f);
				if (stickTimer == 1)
				{
					GetWorldTimerManager().ClearTimer(stickDownHandle);
				}
			}), GetWorld()->GetDeltaSeconds(), true, 1.f);
	}
}

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
	}
}

