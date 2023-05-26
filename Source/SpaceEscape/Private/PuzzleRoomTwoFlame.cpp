// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoFlame.h"

#include "EscapePlayer.h"
#include "PuzzleRoomTwoFlameHitPoint.h"
#include "RoomManager.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APuzzleRoomTwoFlame::APuzzleRoomTwoFlame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);

	flameComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("flameComp"));
	flameComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UParticleSystem>tempFlame(TEXT("/Script/Engine.ParticleSystem'/Game/LTG/Materials/CP_Flame.CP_Flame'"));
    if (tempFlame.Succeeded())
    {
		flameComp->SetTemplate(tempFlame.Object);
    }

	sphereCompForTrace = CreateDefaultSubobject<USphereComponent>(TEXT("CreateDefaultSubobject"));
	sphereCompForTrace->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APuzzleRoomTwoFlame::BeginPlay()
{
	Super::BeginPlay();

	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));
	hitPoint = Cast<APuzzleRoomTwoFlameHitPoint>(UGameplayStatics::GetActorOfClass(this, APuzzleRoomTwoFlameHitPoint::StaticClass()));
}

// Called every frame
void APuzzleRoomTwoFlame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 해당 스테이지 진입시만 동작하게 처리 (나중에)
	if (!bIsClosed)
	{
		DoTrace();
	}	
}

void APuzzleRoomTwoFlame::DoTrace()
{
	FVector offset = (hitPoint->GetActorLocation() - sphereCompForTrace->GetComponentLocation()).GetSafeNormal() * 200;
	FHitResult hitResult;
	FVector startLoc = sphereCompForTrace->GetComponentLocation() + offset;
	FVector endLoc = hitPoint->GetActorLocation();
	TArray<AActor*> forIgnore;
	UKismetSystemLibrary::SphereTraceSingle(this, startLoc, endLoc, 64.f, UEngineTypes::ConvertToTraceType(ECC_Pawn), false, forIgnore, EDrawDebugTrace::ForOneFrame, hitResult, true);

	// 만약 플레이어가 닿았다면
	AEscapePlayer* pl = Cast<AEscapePlayer>(hitResult.GetActor());
	if (pl)
	{
		// 플레이어 죽음 처리
		pl->Die();
		//UE_LOG(LogTemp, Warning, TEXT("You Died"));
	}
}

void APuzzleRoomTwoFlame::CloseValve(float degree)
{
	if (bIsClosed)
	{
		return;
	}
	degreeToClose += (degree / 720.f);
	if (degreeToClose > 0.95)
	{
		degreeToClose = 1;
		bIsClosed = true;
	}
	flameComp->SetRelativeScale3D(FMath::Lerp(FVector(1.f), FVector(0), degreeToClose));
}

