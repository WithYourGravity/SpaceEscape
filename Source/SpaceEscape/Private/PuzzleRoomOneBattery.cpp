// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneBattery.h"

#include "Components/BoxComponent.h"

APuzzleRoomOneBattery::APuzzleRoomOneBattery()
{
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision For Battery"));
	SetRootComponent(boxComp);
}
/*
void APuzzleRoomOneBattery::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneBattery::BeginOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &APuzzleRoomOneBattery::EndOverlap);
}

// 배터리가 범위안으로 들어오면 장착가능하게 하는 함수
void APuzzleRoomOneBattery::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	battery = Cast<ABattery>(OtherActor);
	if (battery)
	{
		bCanSetBattery = true;
	}
}

// 배터리가 범위밖으로 나가면 장착불가능하게 하는 함수
void APuzzleRoomOneBattery::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bCanSetBattery && battery != nullptr)
	{
		bCanSetBattery = false;
		battery = nullptr;
	}
}

// 만약 배터리가 장착가능하다면 배터리를 장착하는 함수
void APuzzleRoomOneBattery::SetBattery()
{
	if (bCanSetBattery)
	{
		// 배터리를 정해진 위치로 이동함

		ReportClear();
	}
}
*/