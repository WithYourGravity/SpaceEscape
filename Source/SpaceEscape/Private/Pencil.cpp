// Fill out your copyright notice in the Description page of Project Settings.


#include "Pencil.h"
#include "Clipboard.h"
#include "GrabComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APencil::APencil()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;
	boxComp->SetBoxExtent(FVector(12.5f, 0.6f, 0.6f));
	boxComp->SetSimulatePhysics(true);
	boxComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/Pencil_Cylinder.Pencil_Cylinder'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		meshComp->SetRelativeScale3D(FVector(0.01f));
		meshComp->SetCollisionProfileName(FName("NoCollision"));
	}

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowComp"));
	arrowComp->SetupAttachment(RootComponent);
	arrowComp->SetRelativeLocation(FVector(9.0f, 0.0f, 0.0f));
	arrowComp->SetRelativeScale3D(FVector(0.05f));

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->SetRelativeRotation(FRotator(0.0f, 180.0f, 180.0f));
	grabComp->grabType = EGrabType::SNAP;
}

// Called when the game starts or when spawned
void APencil::BeginPlay()
{
	Super::BeginPlay();

	grabComp->onGrabbedDelegate.AddUFunction(this, FName("OnGrabbed"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("OnDropped"));
}

// Called every frame
void APencil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUsing)
	{
		DetectHitBoard();
	}
}

void APencil::OnGrabbed()
{
	bUsing = true;
}

void APencil::OnDropped()
{
	bUsing = false;
}

void APencil::DetectHitBoard()
{
	FHitResult hitInfo;
	FVector start = arrowComp->GetComponentLocation();
	FVector end = start + arrowComp->GetForwardVector() * 5.0f;
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

