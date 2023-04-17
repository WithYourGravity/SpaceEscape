// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoteControlObject.h"

#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"


// Sets default values
ARemoteControlObject::ARemoteControlObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(meshComp);
	meshComp->SetRelativeScale3D(FVector(0.025f));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempSkeletal(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_MiniCar.SM_MiniCar'"));
    if (tempSkeletal.Succeeded())
    {
		meshComp->SetStaticMesh(tempSkeletal.Object);
    }
	meshComp->SetSimulatePhysics(true);
	meshComp->SetCollisionProfileName(TEXT("BlockAll"));

	cameraCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camaera Capture Component"));
	cameraCaptureComp->SetupAttachment(RootComponent);
	cameraCaptureComp->SetRelativeLocation(FVector(400.f, 0, 500.f));
}

// Called when the game starts or when spawned
void ARemoteControlObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARemoteControlObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARemoteControlObject::ControlManager(FString input)
{
	if (input == "G")
	{
		GetWorldTimerManager().SetTimer(moveHandle, this, &ARemoteControlObject::MoveForward, 0.1, true);
	}
	else if (input == "B")
	{
		GetWorldTimerManager().SetTimer(moveHandle, this, &ARemoteControlObject::MoveBackward, 0.1, true);
	}
	else if (input == "L")
	{
		GetWorldTimerManager().SetTimer(turnHandle, this, &ARemoteControlObject::TurnLeft, 0.1, true);
	}
	else if (input == "R")
	{
		GetWorldTimerManager().SetTimer(turnHandle, this, &ARemoteControlObject::TurnRight, 0.1, true);
	}
}

void ARemoteControlObject::MoveForward()
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * moveSpeed * GetWorld()->DeltaTimeSeconds);
}

void ARemoteControlObject::MoveBackward()
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * -1 * moveSpeed * GetWorld()->DeltaTimeSeconds);
}

void ARemoteControlObject::TurnRight()
{
	FRotator rot = GetActorRotation();
	rot.Yaw += turnSpeed * GetWorld()->DeltaTimeSeconds;
	SetActorRotation(rot);
}

void ARemoteControlObject::TurnLeft()
{
	FRotator rot = GetActorRotation();
	rot.Yaw += turnSpeed * -1 * GetWorld()->DeltaTimeSeconds;
	SetActorRotation(rot);
}

void ARemoteControlObject::StopMove()
{
	GetWorldTimerManager().ClearTimer(moveHandle);
}

void ARemoteControlObject::StopTurn()
{
	GetWorldTimerManager().ClearTimer(turnHandle);
}


