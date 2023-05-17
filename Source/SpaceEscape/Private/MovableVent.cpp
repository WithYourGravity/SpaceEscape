// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableVent.h"
#include "Components/BoxComponent.h"
#include "GrabComponent.h"

// Sets default values
AMovableVent::AMovableVent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Vent Collision"));
	SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VentMesh"));
	meshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> vMesh(TEXT("/Script/Engine.StaticMesh'/Game/Yeni/Assets/ModularScifiProps/Meshes/SM_Vent_A.SM_Vent_A'"));
	if(vMesh.Succeeded())
	{
		meshComp->SetStaticMesh(vMesh.Object);
	}
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::FREE;
}

// Called when the game starts or when spawned
void AMovableVent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovableVent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

