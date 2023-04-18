// Fill out your copyright notice in the Description page of Project Settings.


#include "WallPad.h"

#include "Components/WidgetComponent.h"

// Sets default values
AWallPad::AWallPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pad Mesh"));
	SetRootComponent(meshComp);
	meshComp->SetRelativeScale3D(FVector(0.2f));
	meshComp->SetSimulatePhysics(false);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/sci-fi-tablet/source/Tablet/Tablet.Tablet'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	wallScreenComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Wall Screen Widget"));
	wallScreenComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWallPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

