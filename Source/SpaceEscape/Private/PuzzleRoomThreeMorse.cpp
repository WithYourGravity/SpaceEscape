// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorse.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
APuzzleRoomThreeMorse::APuzzleRoomThreeMorse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SetRootComponent(sceneComp);
	buttonBodyComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("buttonBodyComp"));
	buttonBodyComp->SetupAttachment(RootComponent);
	buttonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("buttonComp"));
	buttonComp->SetupAttachment(buttonBodyComp);
	leverBodyComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverBodyComp"));
	leverBodyComp->SetupAttachment(RootComponent);
	leverComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverComp"));
	leverComp->SetupAttachment(leverBodyComp);
	leverCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("leverCollision"));
	leverCollision->SetupAttachment(leverComp);
	screenComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("screenComp"));
	screenComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APuzzleRoomThreeMorse::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuzzleRoomThreeMorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsGrabed)
	{
		LeverShouldBeZero();
	}
}

// 레버 틱마다 원위치로 이동시키는 함수
void APuzzleRoomThreeMorse::LeverShouldBeZero()
{
	FRotator rot = leverComp->GetRelativeRotation();

	if (rot.Roll > 5)
	{
		rot.Roll -= 5;
		leverComp->SetRelativeLocationAndRotation(FVector(0), rot);
	}
	else
	{
		rot.Roll = 0;
		leverComp->SetRelativeLocationAndRotation(FVector(0), rot);
	}
}

