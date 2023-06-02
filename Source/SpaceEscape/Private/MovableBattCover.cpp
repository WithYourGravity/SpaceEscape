// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableBattCover.h"
#include "Screw.h"

// 배터리 커버
AMovableBattCover::AMovableBattCover()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ConstructorHelpers::FObjectFinder<UStaticMesh> vMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube_Trim.Shape_Cube_Trim'"));
	if (vMesh.Succeeded())
	{
		meshComp->SetStaticMesh(vMesh.Object);
	}
	meshComp->SetRelativeLocation(FVector(0, 0, -54.847001f));
	meshComp->SetRelativeScale3D(FVector(0.02f, 1, 1));

	warningSignMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("warningSignMesh"));
	warningSignMesh->SetupAttachment(RootComponent);
	warningSignMesh->SetRelativeLocation(FVector(-1.218733f, 0, -1.472553));
	warningSignMesh->SetRelativeRotation(FRotator(0, 90, 90));
	warningSignMesh->SetRelativeScale3D(FVector(0.37f, 0.35f, 0.35f));

	handleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("handleMesh"));
	handleMesh->SetupAttachment(RootComponent);
	handleMesh->SetRelativeLocationAndRotation(FVector(0.297674f, -43.351612f, -29.834242f), FRotator(0, -180, -90));
	handleMesh->SetRelativeScale3D(FVector(0.2f, 1, 1));
}

// Called when the game starts or when spawned
void AMovableBattCover::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMovableBattCover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovableBattCover::FallingCover()
{
	Super::FallingCover();
	//배터리에 델리게이트 보내기
	if (screw->bAllFallenScrews == true)
	{		
		allowBatt.Execute();
		UE_LOG(LogTemp, Warning, TEXT("AMovable Battery Cover FallingCover()"))
	}
}
