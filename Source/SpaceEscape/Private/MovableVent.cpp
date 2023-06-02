// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableVent.h"
#include "Components/BoxComponent.h"
#include "GrabComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Screw.h"

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

	Tags.Add(FName("Sense"));
	meshComp->ComponentTags.Add(FName("Sense.R1"));
}

// Called when the game starts or when spawned
void AMovableVent::BeginPlay()
{
	Super::BeginPlay();

	screw = Cast<AScrew>(UGameplayStatics::GetActorOfClass(GetWorld(), AScrew::StaticClass()));
}

// Called every frame
void AMovableVent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AMovableVent::FallingVent()
//{
//	if (cameOutScrewCount < 4 )
//	{
//		UE_LOG(LogTemp, Warning, TEXT("isEnoughRotated"))
//		cameOutScrewCount++;
//		UE_LOG(LogTemp, Warning, TEXT("cameOutScrew Count : %d"), cameOutScrewCount)
//		boxComp->SetSimulatePhysics(false);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("cameOutScrew Count222 : %d"), cameOutScrewCount)
//		boxComp->SetSimulatePhysics(true);
//	}
//}

