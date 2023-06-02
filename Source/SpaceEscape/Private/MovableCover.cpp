// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableCover.h"
#include "Components/BoxComponent.h"
#include "GrabComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Screw.h"

// Moveable Covers Parents
AMovableCover::AMovableCover()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Vent Collision"));
	SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Movable Cover Mesh"));
	meshComp->SetupAttachment(RootComponent);
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::NONE;

	Tags.Add(FName("Sense"));
	meshComp->ComponentTags.Add(FName("Sense.R1"));
}

// Called when the game starts or when spawned
void AMovableCover::BeginPlay()
{
	Super::BeginPlay();

	screw = Cast<AScrew>(UGameplayStatics::GetActorOfClass(GetWorld(), AScrew::StaticClass()));
}

// Called every frame
void AMovableCover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovableCover::FallingCover()
{
	UE_LOG(LogTemp, Warning, TEXT("AMovableCover::FallingCover bAllFallenScrews : %d"), screw->bAllFallenScrews);
	if(screw->bAllFallenScrews == true)
	{
		boxComp->SetSimulatePhysics(true);
		//나사가 다 빠졌을 때 Grab할 수 있다.
		grabComp->grabType = EGrabType::FREE;
	}

}


