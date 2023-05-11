// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeJoystick.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "MotionControllerComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APuzzleRoomThreeJoystick::APuzzleRoomThreeJoystick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);

	baseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("baseMeshComp"));
	baseMeshComp->SetupAttachment(RootComponent);
	baseMeshComp->SetCollisionProfileName(FName("NoCollision"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempBaseMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_JoystickBase.SM_JoystickBase'"));
    if (tempBaseMesh.Succeeded())
    {
		baseMeshComp->SetStaticMesh(tempBaseMesh.Object);
    }

	stickMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("stickMeshComp"));
	stickMeshComp->SetupAttachment(RootComponent);
	stickMeshComp->SetRelativeLocation(FVector(0, 0, 15.f));
	stickMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempStickMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_Joystick.SM_Joystick'"));
	if (tempStickMesh.Succeeded())
	{
		stickMeshComp->SetStaticMesh(tempStickMesh.Object);
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::LEVER;
	grabComp->SetRelativeLocation(FVector(0, 0, 50.f));
}

// Called when the game starts or when spawned
void APuzzleRoomThreeJoystick::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	grabComp->onGrabbedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));
	grabComp->onDroppedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));
}

// Called every frame
void APuzzleRoomThreeJoystick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APuzzleRoomThreeJoystick::ChangeIsGrabed()
{
	bIsGrabed = !bIsGrabed;
	if (bIsGrabed)
	{
		ControlByPlayerHand();
	}
	else
	{
		SetStickDefault();
	}
}

void APuzzleRoomThreeJoystick::ControlByPlayerHand()
{

	player->rightHandMesh->SetWorldLocation(grabComp->GetComponentLocation());
	//player->rightHandMesh->AttachToComponent(grabComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void APuzzleRoomThreeJoystick::SetStickDefault()
{
	player->rightHandMesh->SetWorldLocation(player->rightHand->GetComponentLocation());
}

