// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrewDriver.h"
#include "Components/BoxComponent.h"
#include "Screw.h"
#include "GrabComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AScrewDriver::AScrewDriver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Screw Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(0.8, 0.7, 0.8));
	boxComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewMesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetWorldRotation(FRotator(0, 90, 0));
	ConstructorHelpers::FObjectFinder<UStaticMesh> dMesh(TEXT("/Script/Engine.StaticMesh'/Game/Deko_MatrixDemo/Apartment/Meshes/SM_ScrewDriver_A01_N1.SM_ScrewDriver_A01_N1'"));
	if(dMesh.Succeeded())
	{
		meshComp->SetStaticMesh(dMesh.Object);
		meshComp->SetRelativeLocation(FVector(-9, 0, 0));
		meshComp->SetRelativeRotation(FRotator(0, 90, 180));
		meshComp->SetRelativeScale3D(FVector(1));
	}
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::FREE;
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	arrowComp->SetupAttachment(RootComponent);

	Tags.Add(FName("Sense"));
	meshComp->ComponentTags.Add(FName("Sense.R1"));
}

// Called when the game starts or when spawned
void AScrewDriver::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScrew::StaticClass(), screwActors);
	screw = Cast<AScrew>(UGameplayStatics::GetActorOfClass(GetWorld(), AScrew::StaticClass()));
}

// Called every frame
void AScrewDriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

}


