// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoteControlObject.h"

#include "GrabComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

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
	meshComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));

	cameraCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera Capture Component"));
	cameraCaptureComp->SetupAttachment(RootComponent);
	cameraCaptureComp->SetRelativeLocation(FVector(400.f, 0, 500.f));
	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D>tempTarget(TEXT("/Script/Engine.TextureRenderTarget2D'/Game/LTG/Materials/RT_RemoteRenderTarget.RT_RemoteRenderTarget'"));
    if (tempTarget.Succeeded())
    {
		cameraCaptureComp->TextureTarget = tempTarget.Object;
    }

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::FREE;

	soundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("soundComp"));
	soundComp->SetupAttachment(RootComponent);
	soundComp->SetAutoActivate(false);
	ConstructorHelpers::FObjectFinder<USoundWave>tempSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/remote_object_car.remote_object_car'"));
    if (tempSound.Succeeded())
    {
		soundComp->SetSound(tempSound.Object);
    }

	Tags.Add(FName("Sense"));
	meshComp->ComponentTags.Add(FName("Sense.R1"));
}

// Called when the game starts or when spawned
void ARemoteControlObject::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(upCheckhandle, this, &ARemoteControlObject::CheckIsUp, 1.f, true);
}

// Called every frame
void ARemoteControlObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARemoteControlObject::ControlManager(FString input)
{
	if (!bcanMove)
	{
		return;
	}

	if (input == "G")
	{
		GetWorldTimerManager().SetTimer(moveHandle, this, &ARemoteControlObject::MoveForward, GetWorld()->DeltaTimeSeconds, true);
	}
	else if (input == "B")
	{
		GetWorldTimerManager().SetTimer(moveHandle, this, &ARemoteControlObject::MoveBackward, GetWorld()->DeltaTimeSeconds, true);
	}
	else if (input == "L")
	{
		GetWorldTimerManager().SetTimer(turnHandle, this, &ARemoteControlObject::TurnLeft, GetWorld()->DeltaTimeSeconds, true);
	}
	else if (input == "R")
	{
		GetWorldTimerManager().SetTimer(turnHandle, this, &ARemoteControlObject::TurnRight, GetWorld()->DeltaTimeSeconds, true);
	}

	soundComp->Play();
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
	soundComp->Stop();
}

void ARemoteControlObject::StopTurn()
{
	GetWorldTimerManager().ClearTimer(turnHandle);
	soundComp->Stop();
}

void ARemoteControlObject::CheckIsUp()
{
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::UpVector, GetActorUpVector())));

	if (degree < 45.f)
	{
		bcanMove = true;
	}
	else
	{
		bcanMove = false;
	}
	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
}


