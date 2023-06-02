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

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);
	meshComp->SetSimulatePhysics(true);
	meshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/eraser.eraser'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		meshComp->SetRelativeScale3D(FVector(0.7f, 0.8f, 1.0f));
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::MARKER;

	Tags.Add(FName("Sense"));
	meshComp->ComponentTags.Add(FName("Sense.R2"));
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
	FVector start = grabComp->GetComponentLocation() + grabComp->GetUpVector() * -1.0f;
	FVector end = start + grabComp->GetUpVector() * -3.0f;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.bReturnFaceIndex = true;

	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, -1, 0, -1);

	bool bHit = GetWorld()->LineTraceSingleByProfile(hitInfo, start, end, FName("BoardPreset"), params);

	if (bHit)
	{
		AClipboard* board = Cast<AClipboard>(hitInfo.GetActor());
		if (board)
		{
			float eraserDir = FVector::DotProduct(board->GetActorUpVector(), (end - board->GetActorLocation()));

			FVector handLocation;
			if (grabComp->GetHeldByHand() == EControllerHand::Right)
			{
				handLocation = player->rightHandMesh->GetComponentLocation();
			}
			else
			{
				handLocation = player->leftHandMesh->GetComponentLocation();
			}
			float handDir = FVector::DotProduct(board->GetActorUpVector(), handLocation - board->GetActorLocation());

			if (eraserDir >= 0.0f && handDir >= 0.0f)
			{
				board->OnPaintVisualTraceLine(this, hitInfo);
			}
		}
	}
}

