// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneBattery.h"

#include "Battery.h"
#include "GrabComponent.h"
#include "Components/BoxComponent.h"

APuzzleRoomOneBattery::APuzzleRoomOneBattery()
{
	targerPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("Position For Battery"));
	SetRootComponent(targerPosition);
	targerPosition->SetBoxExtent(FVector(8));
	targerPosition->SetCollisionProfileName(FName("NoCollision"));

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Mountable Range"));
	boxComp->SetupAttachment(RootComponent);
}

void APuzzleRoomOneBattery::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneBattery::BeginOverlap);
}

// 배터리가 범위안으로 들어오면 장착가능하게 하는 함수
void APuzzleRoomOneBattery::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto battery = Cast<ABattery>(OtherActor);
	if (battery)
	{
		battery->grabComp->TryRelease();
		battery->SetActorLocationAndRotation(targerPosition->GetComponentLocation(), FRotator::ZeroRotator);
		battery->meshComp->SetSimulatePhysics(false);
		battery->meshComp->SetCollisionProfileName(FName("NoCollision"));
		boxComp->SetCollisionProfileName(FName("NoCollision"));
		ReportClear();
	}
}