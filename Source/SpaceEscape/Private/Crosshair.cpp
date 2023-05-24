// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"


// Sets default values
ACrosshair::ACrosshair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	defaultSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("defaultSceneComp"));
	RootComponent = defaultSceneComp;

	crosshairComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("crosshairComp"));
	crosshairComp->SetupAttachment(RootComponent);

	// static mesh 설정
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (tempMesh.Succeeded())
	{
		crosshairComp->SetStaticMesh(tempMesh.Object);
	}
	// material 설정
	ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial(TEXT("/Script/Engine.Material'/Game/YSY/Assets/Crosshair/M_Crosshair.M_Crosshair'"));
	if (tempMaterial.Succeeded())
	{
		crosshairComp->SetMaterial(0, tempMaterial.Object);
	}

	crosshairComp->SetCollisionProfileName(FName("NoCollision"));
	crosshairComp->SetRelativeRotation(FRotator(90, 0, 0));
	crosshairComp->SetRelativeScale3D(FVector(0.05f));
	crosshairComp->SetVisibility(false);

}
// Called when the game starts or when spawned
void ACrosshair::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrosshair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

