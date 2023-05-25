// Fill out your copyright notice in the Description page of Project Settings.


#include "Eraser.h"
#include "Clipboard.h"
#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEraser::AEraser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	//SetRootComponent(boxComp);
	//boxComp->SetBoxExtent(FVector(6.5f, 3.0f, 1.5f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);
	//meshComp->SetupAttachment(RootComponent);
	meshComp->SetSimulatePhysics(true);
	meshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/Yuta_Eraser_Mesh.Yuta_Eraser_Mesh'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		//meshComp->SetRelativeLocation(FVector(27.5f, -25.5f, 14.5f));
		//meshComp->SetRelativeRotation(FRotator(-5.0f, 50.0f, 35.0f));
		//meshComp->SetRelativeScale3D(FVector(0.001f));
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(RootComponent);
	//grabComp->SetRelativeLocation(FVector(27.5f, -25.5f, 14.5f));
	//grabComp->SetRelativeRotation(FRotator(-5.0f, 50.0f, 35.0f));
	grabComp->grabType = EGrabType::MARKER;
}

// Called when the game starts or when spawned
void AEraser::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	grabComp->onGrabbedDelegate.AddUFunction(this, FName("OnGrabbed"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("OnDropped"));
}

// Called every frame
void AEraser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUsing)
	{
		DetectHitBoard();
	}
}

void AEraser::OnGrabbed()
{
	bUsing = true;

	if (grabComp->GetHeldByHand() == EControllerHand::Right)
	{
		AttachToComponent(player->rightHandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("eraserSocket_r"));
	}
	else
	{
		AttachToComponent(player->leftHandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("eraserSocket_l"));
	}
}

void AEraser::OnDropped()
{
	bUsing = false;
}

void AEraser::DetectHitBoard()
{
	FHitResult hitInfo;
	FVector start = GetActorLocation();
	FVector end = start + GetActorUpVector() * -2.0f;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.bReturnFaceIndex = true;

	bool bHit = GetWorld()->LineTraceSingleByProfile(hitInfo, start, end, FName("BoardPreset"), params);

	if (bHit)
	{
		AClipboard* board = Cast<AClipboard>(hitInfo.GetActor());
		if (board)
		{
			board->OnPaintVisualTraceLine(this, hitInfo);
		}
	}
}

