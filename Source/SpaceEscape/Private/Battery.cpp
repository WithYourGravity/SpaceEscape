// Fill out your copyright notice in the Description page of Project Settings.


#include "Battery.h"

#include "GrabComponent.h"

// Sets default values
ABattery::ABattery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(meshComp);
	meshComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	meshComp->SetSimulatePhysics(true);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_Battery.SM_Battery'"));
    if (tempMesh.Succeeded())
    {
		meshComp->SetStaticMesh(tempMesh.Object);
    }

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(meshComp);
	grabComp->grabType = EGrabType::FREE;

	Tags.Add(FName("Sense"));
	meshComp->ComponentTags.Add(FName("Sense.R1"));
}

// Called when the game starts or when spawned
void ABattery::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

