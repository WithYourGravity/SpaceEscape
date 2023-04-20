// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorse.h"

#include "LeverComponent.h"
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
	buttonBodyComp->SetCollisionProfileName(TEXT("NoCollision"));
	buttonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("buttonComp"));
	buttonComp->SetupAttachment(buttonBodyComp);
	buttonComp->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	screenComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("screenComp"));
	screenComp->SetupAttachment(RootComponent);
	//leverComp = CreateDefaultSubobject<ULeverComponent>(TEXT("LeverComp"));
	//leverComp->SetupAttachment(RootComponent);
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

}
