// Fill out your copyright notice in the Description page of Project Settings.


#include "Clipboard.h"
#include "GrabComponent.h"
#include "Marker.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

// Sets default values
AClipboard::AClipboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boardMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("boardMeshComp"));
	SetRootComponent(boardMeshComp);
	boardMeshComp->SetSimulatePhysics(true);
	boardMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempBoardMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/ClipBoard_board_1.ClipBoard_board_1'"));
	if (tempBoardMesh.Succeeded())
	{
		boardMeshComp->SetStaticMesh(tempBoardMesh.Object);
		boardMeshComp->SetRelativeScale3D(FVector(0.5f));
	}

	pageMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pageMeshComp"));
	pageMeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempPageMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/ClipBoard_page.ClipBoard_page'"));
	if (tempPageMesh.Succeeded())
	{
		pageMeshComp->SetStaticMesh(tempPageMesh.Object);
		pageMeshComp->SetCollisionProfileName(FName("BoardPreset"));
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> tempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/YSY/Assets/BoardNPencil/MI_Page.MI_Page'"));
	if (tempMat.Succeeded())
	{
		pageMeshComp->SetMaterial(0, tempMat.Object);
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::FREE;
	grabComp->SetRelativeLocation(FVector(0.0f, 24.0f, 4.0f));
	grabComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	ConstructorHelpers::FObjectFinder<UMaterialInstance> tempPageMaterialInst(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/YSY/Assets/BoardNPencil/MI_Page.MI_Page'"));
	if (tempPageMaterialInst.Succeeded())
	{
		pageMaterialInst = tempPageMaterialInst.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> tempPaintBrushMaterialInterface(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/YSY/Assets/BoardNPencil/MI_PaintBrush.MI_PaintBrush'"));
	if (tempPaintBrushMaterialInterface.Succeeded())
	{
		paintBrushMaterialInterface = tempPaintBrushMaterialInterface.Object;
	}
}

// Called when the game starts or when spawned
void AClipboard::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic* pageDynamicMaterial = pageMeshComp->CreateDynamicMaterialInstance(0, pageMaterialInst);

	renderToTexture = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), renderTextureSizeX, renderTextureSizeY);

	brushMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), paintBrushMaterialInterface);

	pageDynamicMaterial->SetTextureParameterValue(FName("BrushStroke"), renderToTexture);
}

// Called every frame
void AClipboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClipboard::OnPaintVisualTraceLine(AMarker* brush, const FHitResult& hitInfo)
{
	marker = brush;

	FVector2D collisionUV;
	UGameplayStatics::FindCollisionUV(hitInfo, 0, collisionUV);

	brushMaterialInstance->SetVectorParameterValue(FName("HitPosition"), FVector4(collisionUV.X, collisionUV.Y, 0.0f, 1.0f));
	brushMaterialInstance->SetScalarParameterValue(FName("BrushSize"), marker->brushSize);
	brushMaterialInstance->SetVectorParameterValue(FName("BrushColor"), marker->brushColor);
	brushMaterialInstance->SetScalarParameterValue(FName("BrushStrength"), marker->brushStrength);

	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), renderToTexture, brushMaterialInstance);
}
