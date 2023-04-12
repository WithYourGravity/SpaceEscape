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

// ���͸��� ���������� ������ ���������ϰ� �ϴ� �Լ�
void APuzzleRoomOneBattery::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	battery = Cast<ABattery>(OtherActor);
	if (battery)
	{
		bCanSetBattery = true;
	}
}

// ���͸��� ���������� ������ �����Ұ����ϰ� �ϴ� �Լ�
void APuzzleRoomOneBattery::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bCanSetBattery && battery != nullptr)
	{
		bCanSetBattery = false;
		battery = nullptr;
	}
}

// ���� ���͸��� ���������ϴٸ� ���͸��� �����ϴ� �Լ�
void APuzzleRoomOneBattery::SetBattery()
{
	if (bCanSetBattery)
	{
		// ���͸��� ������ ��ġ�� �̵���

		ReportClear();
	}
}
*/