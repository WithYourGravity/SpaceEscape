// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneIDCard.h"
#include "GrabComponent.h"

// Sets default values
APuzzleRoomOneIDCard::APuzzleRoomOneIDCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	idCardComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("idImage"));
	SetRootComponent(idCardComp);
	idCardComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	idCardComp->SetSimulatePhysics(true);
	idCardBwdComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("idBwdImage"));
	idCardBwdComp->SetupAttachment(RootComponent);
	boxComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("idBody"));
	boxComp->SetupAttachment(RootComponent);
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::FREE;
}

// Called when the game starts or when spawned
void APuzzleRoomOneIDCard::BeginPlay()
{
	Super::BeginPlay();

	
}


void APuzzleRoomOneIDCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

