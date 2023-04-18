// Fill out your copyright notice in the Description page of Project Settings.


#include "Doors.h"
#include "Components/BoxComponent.h"

// Sets default values
ADoors::ADoors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	leftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftDoorMesh"));
	leftDoorMesh->SetRelativeLocation(FVector(0, -463, 0));
	leftDoorMesh->SetRelativeScale3D(FVector(1, 4.625f, 2.4f));
	rightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightDoorMesh"));
	rightDoorMesh->SetRelativeLocation(FVector(0, 0, 0));
	rightDoorMesh->SetRelativeScale3D(FVector(1, 4.625f, 2.4f));

}

// Called when the game starts or when spawned
void ADoors::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void ADoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

