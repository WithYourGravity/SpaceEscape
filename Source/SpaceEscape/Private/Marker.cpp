// Fill out your copyright notice in the Description page of Project Settings.


#include "Marker.h"
#include "Clipboard.h"
#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMarker::AMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootCylinderMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rootCylinderMeshComp"));
	SetRootComponent(rootCylinderMeshComp);
	rootCylinderMeshComp->SetSimulatePhysics(true);
	rootCylinderMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempRootCylinderMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/GFX_Cylinder.GFX_Cylinder'"));
	if (tempRootCylinderMesh.Succeeded())
	{
		rootCylinderMeshComp->SetStaticMesh(tempRootCylinderMesh.Object);
	}

	cylinderMeshComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cylinderMeshComp1"));
	cylinderMeshComp1->SetupAttachment(RootComponent);
	cylinderMeshComp1->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempCylinderMesh1(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/GFX_Cylinder_1.GFX_Cylinder_1'"));
	if (tempCylinderMesh1.Succeeded())
	{
		cylinderMeshComp1->SetStaticMesh(tempCylinderMesh1.Object);
	}

	cylinderMeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cylinderMeshComp2"));
	cylinderMeshComp2->SetupAttachment(RootComponent);
	cylinderMeshComp2->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempCylinderMesh2(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/GFX_Cylinder_2.GFX_Cylinder_2'"));
	if (tempCylinderMesh2.Succeeded())
	{
		cylinderMeshComp2->SetStaticMesh(tempCylinderMesh2.Object);
	}

	cylinderMeshComp3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cylinderMeshComp3"));
	cylinderMeshComp3->SetupAttachment(RootComponent);
	cylinderMeshComp3->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempCylinderMesh3(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/GFX_Cylinder_3.GFX_Cylinder_3'"));
	if (tempCylinderMesh3.Succeeded())
	{
		cylinderMeshComp3->SetStaticMesh(tempCylinderMesh3.Object);
	}

	tipMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("tipMeshComp"));
	tipMeshComp->SetupAttachment(RootComponent);
	tipMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempTipMeshComp(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/GFX_Tip.GFX_Tip'"));
	if (tempTipMeshComp.Succeeded())
	{
		tipMeshComp->SetStaticMesh(tempTipMeshComp.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> tempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/YSY/Assets/BoardNPencil/MI_ColorPencil.MI_ColorPencil'"));
	if (tempMat.Succeeded())
	{
		tipMeshComp->SetMaterial(0, tempMat.Object);
		cylinderMeshComp1->SetMaterial(0, tempMat.Object);

		colorMaterialInst = tempMat.Object;
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::MARKER;

	Tags.Add(FName("Sense"));
	rootCylinderMeshComp->ComponentTags.Add(FName("Sense.R2"));
}

// Called when the game starts or when spawned
void AMarker::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	grabComp->onGrabbedDelegate.AddUFunction(this, FName("OnGrabbed"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("OnDropped"));

	UMaterialInstanceDynamic* tipColorDynamicMaterial = tipMeshComp->CreateDynamicMaterialInstance(0, colorMaterialInst);
	tipColorDynamicMaterial->SetVectorParameterValue(FName("PencilColor"), brushColor);
	UMaterialInstanceDynamic* cylinderColorDynamicMaterial = cylinderMeshComp1->CreateDynamicMaterialInstance(0, colorMaterialInst);
	cylinderColorDynamicMaterial->SetVectorParameterValue(FName("PencilColor"), brushColor);
}

// Called every frame
void AMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUsing)
	{
		DetectHitBoard();
	}
}

void AMarker::OnGrabbed()
{
	bUsing = true;

	if (grabComp->GetHeldByHand() == EControllerHand::Right)
	{
		AttachToComponent(player->rightHandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("markerSocket_r"));
	}
	else
	{
		AttachToComponent(player->leftHandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("markerSocket_l"));
	}
}

void AMarker::OnDropped()
{
	bUsing = false;
}

void AMarker::DetectHitBoard()
{
	FHitResult hitInfo;
	FVector start = tipMeshComp->GetComponentLocation();
	FVector end = start + tipMeshComp->GetUpVector() * 3.5f;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.bReturnFaceIndex = true;

	bool bHit = GetWorld()->LineTraceSingleByProfile(hitInfo, start, end, FName("BoardPreset"), params);
	//bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_WorldDynamic, params);

	if (bHit)
	{
		AClipboard* board = Cast<AClipboard>(hitInfo.GetActor());
		if (board)
		{
			float markerDir = FVector::DotProduct(board->GetActorUpVector(), end - board->GetActorLocation());

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

			if (markerDir >= 0.0f && handDir >= 0.0f)
			{
				board->OnPaintVisualTraceLine(this, hitInfo);
			}
		}
	}
	else
	{
		prevCollisionUV = FVector2D(INFINITY);
	}
}

