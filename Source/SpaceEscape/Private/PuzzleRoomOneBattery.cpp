// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneBattery.h"

#include "Battery.h"
#include "GrabComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MovableBattCover.h"

APuzzleRoomOneBattery::APuzzleRoomOneBattery()
{
	targerPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("Position For Battery"));
	SetRootComponent(targerPosition);
	targerPosition->SetBoxExtent(FVector(8));
	targerPosition->SetCollisionProfileName(FName("NoCollision"));

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Mountable Range"));
	boxComp->SetupAttachment(RootComponent);

	cableMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cableMeshComp"));
	cableMeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempCableMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Wire/SM_WVD_Wires_A.SM_WVD_Wires_A'"));
	if (tempCableMesh.Succeeded())
	{
		cableMeshComp->SetStaticMesh(tempCableMesh.Object);
	}
	cableMeshComp->SetRelativeScale3D(FVector(0.5f));
	cableMeshComp->SetRelativeLocation(FVector(-26.f, -5.f, 21.f));

	ConstructorHelpers::FObjectFinder<USoundBase>tempSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/BatteryChargingSound.BatteryChargingSound'"));
	if (tempSound.Succeeded())
	{
		batterySound = tempSound.Object;
	}
}

void APuzzleRoomOneBattery::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneBattery::BeginOverlap);
	myCover = Cast<AMovableBattCover>(UGameplayStatics::GetActorOfClass(this, AMovableBattCover::StaticClass()));
	if (myCover)
	{
		myCover->allowBatt.BindUFunction(this, FName("WhenScrewFinished"));
	}
}

// 배터리가 범위안으로 들어오면 장착가능하게 하는 함수
void APuzzleRoomOneBattery::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsScrewFinished)
	{
		return;
	}

	auto battery = Cast<ABattery>(OtherActor);
	if (battery)
	{
		battery->grabComp->TryRelease();
		battery->SetActorLocationAndRotation(targerPosition->GetComponentLocation() + FVector(0, 0, -22.5), FRotator::ZeroRotator);
		battery->meshComp->SetSimulatePhysics(false);
		battery->meshComp->SetCollisionProfileName(FName("NoCollision"));
		boxComp->SetCollisionProfileName(FName("NoCollision"));
		ReportClear();

		// 배터리 사운드 재생
		UGameplayStatics::PlaySound2D(this, batterySound);
	}
}

void APuzzleRoomOneBattery::WhenScrewFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("APuzzleRoomOneBattery::WhenScrewFinished"));
	bIsScrewFinished = true;
}
