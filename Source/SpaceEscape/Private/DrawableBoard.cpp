// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawableBoard.h"
#include "Eraser.h"
#include "GrabComponent.h"
#include "Marker.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

// Sets default values
ADrawableBoard::ADrawableBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boardMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("boardMeshComp"));
	SetRootComponent(boardMeshComp);
	boardMeshComp->SetSimulatePhysics(true);
	boardMeshComp->SetCollisionProfileName(FName("BoardPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempBoardMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/BoardNPencil/WhiteBoard/interactive1.interactive1'"));
	if (tempBoardMesh.Succeeded())
	{
		boardMeshComp->SetStaticMesh(tempBoardMesh.Object);
		//boardMeshComp->SetRelativeScale3D(FVector(0.5f));
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> tempMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/YSY/Assets/BoardNPencil/MI_Page.MI_Page'"));
	if (tempMat.Succeeded())
	{
		boardMeshComp->SetMaterial(0, tempMat.Object);
	}

	//grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	//grabComp->SetupAttachment(RootComponent);
	//grabComp->grabType = EGrabType::FREE;

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

	ConstructorHelpers::FObjectFinder<UMaterialInterface> tempEraseBrushMaterialInterface(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/YSY/Assets/BoardNPencil/MI_EraseBrush.MI_EraseBrush'"));
	if (tempEraseBrushMaterialInterface.Succeeded())
	{
		eraseBrushMaterialInterface = tempEraseBrushMaterialInterface.Object;
	}

	Tags.Add(FName("Sense"));
	boardMeshComp->ComponentTags.Add(FName("Sense.R2"));
}

// Called when the game starts or when spawned
void ADrawableBoard::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic* pageDynamicMaterial = boardMeshComp->CreateDynamicMaterialInstance(0, pageMaterialInst);

	renderToTexture = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), renderTextureSizeX, renderTextureSizeY);

	brushMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), paintBrushMaterialInterface);
	eraseBrushMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), eraseBrushMaterialInterface);

	pageDynamicMaterial->SetTextureParameterValue(FName("BrushStroke"), renderToTexture);
}

// Called every frame
void ADrawableBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrawableBoard::OnPaintVisualTraceLine(AActor* brush, const FHitResult& hitInfo)
{
	AMarker* marker = Cast<AMarker>(brush);
	AEraser* eraser = Cast<AEraser>(brush);

	FVector2D collisionUV;
	UGameplayStatics::FindCollisionUV(hitInfo, 0, collisionUV);

	//FString s = FString::Printf(TEXT("%f, %f"), collisionUV.X, collisionUV.Y);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, s, true, FVector2D(1.5f));

	if (marker)
	{
		if (FVector2D::Distance(marker->prevCollisionUV, collisionUV) < 0.2f)
		{
			if (!marker->prevCollisionUV.Equals(FVector2D(INFINITY)))
			{
				for (float alpha = 0.001f; alpha < 0.999f; alpha += 0.1f)
				{
					FVector2D subDrawPoint = FMath::Lerp<FVector2D>(marker->prevCollisionUV, collisionUV, alpha);
					brushMaterialInstance->SetVectorParameterValue(FName("HitPosition"), FVector4(subDrawPoint.X, subDrawPoint.Y, 0.0f, 1.0f));
					brushMaterialInstance->SetScalarParameterValue(FName("BrushSize"), marker->brushSize);
					brushMaterialInstance->SetVectorParameterValue(FName("BrushColor"), marker->brushColor);
					brushMaterialInstance->SetScalarParameterValue(FName("BrushStrength"), marker->brushStrength);

					UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), renderToTexture, brushMaterialInstance);
				}
			}
		}
		else
		{
			marker->prevCollisionUV = FVector2D(INFINITY);
		}
	}

	if (marker)
	{
		marker->prevCollisionUV = collisionUV;

		brushMaterialInstance->SetVectorParameterValue(FName("HitPosition"), FVector4(collisionUV.X, collisionUV.Y, 0.0f, 1.0f));
		brushMaterialInstance->SetScalarParameterValue(FName("BrushSize"), marker->brushSize);
		brushMaterialInstance->SetVectorParameterValue(FName("BrushColor"), marker->brushColor);
		brushMaterialInstance->SetScalarParameterValue(FName("BrushStrength"), marker->brushStrength);

		UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), renderToTexture, brushMaterialInstance);
		//UKismetRenderingLibrary::ExportRenderTarget(this, renderToTexture, TEXT("C:/Users/SBAuser/Desktop"), TEXT("rendertarget.png"));
	}
	else if (eraser)
	{
		eraseBrushMaterialInstance->SetVectorParameterValue(FName("HitPosition"), FVector4(collisionUV.X, collisionUV.Y, 0.0f, 1.0f));
		eraseBrushMaterialInstance->SetScalarParameterValue(FName("BrushSize"), eraser->brushSize);
		eraseBrushMaterialInstance->SetVectorParameterValue(FName("BrushColor"), eraser->brushColor);
		eraseBrushMaterialInstance->SetScalarParameterValue(FName("BrushStrength"), eraser->brushStrength);

		UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), renderToTexture, eraseBrushMaterialInstance);
	}
}

