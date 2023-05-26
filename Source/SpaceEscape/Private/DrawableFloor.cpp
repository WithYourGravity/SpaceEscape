// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawableFloor.h"

#include "Eraser.h"
#include "Marker.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

// Sets default values
ADrawableFloor::ADrawableFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	floorMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("floorMeshComp"));
	SetRootComponent(floorMeshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempBoardMesh(TEXT("//Script/Engine.StaticMesh'/Game/Yeni/Assets/VerticalWalls/WallMeshes/SM_Floor_Main.SM_Floor_Main'"));
	if (tempBoardMesh.Succeeded())
	{
		floorMeshComp->SetStaticMesh(tempBoardMesh.Object);
		floorMeshComp->SetRelativeScale3D(FVector(0.7f));
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
}

// Called when the game starts or when spawned
void ADrawableFloor::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic* pageDynamicMaterial = floorMeshComp->CreateDynamicMaterialInstance(4, pageMaterialInst);

	renderToTexture = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), renderTextureSizeX, renderTextureSizeY);

	brushMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), paintBrushMaterialInterface);
	eraseBrushMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), eraseBrushMaterialInterface);

	pageDynamicMaterial->SetTextureParameterValue(FName("BrushStroke"), renderToTexture);
}

// Called every frame
void ADrawableFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrawableFloor::OnPaintVisualTraceLine(AActor* brush, const FHitResult& hitInfo)
{
	AMarker* marker = Cast<AMarker>(brush);
	AEraser* eraser = Cast<AEraser>(brush);

	FVector2D collisionUV;
	UGameplayStatics::FindCollisionUV(hitInfo, 0, collisionUV);

	if (marker)
	{
		if (FVector2D::Distance(prevCollisionUV, collisionUV) > 0.0001f || FVector2D::Distance(prevCollisionUV, collisionUV) < 0.1f)
		{
			UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!!!!!!!!!"));
			brushMaterialInstance->SetVectorParameterValue(FName("HitPosition"), FVector4((prevCollisionUV.X + collisionUV.X) / 2.0f, (prevCollisionUV.Y + collisionUV.Y) / 2.0f, 0.0f, 1.0f));
			brushMaterialInstance->SetScalarParameterValue(FName("BrushSize"), marker->brushSize);
			//brushMaterialInstance->SetVectorParameterValue(FName("BrushColor"), FLinearColor::Black);
			brushMaterialInstance->SetVectorParameterValue(FName("BrushColor"), marker->brushColor);
			brushMaterialInstance->SetScalarParameterValue(FName("BrushStrength"), marker->brushStrength);

			UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), renderToTexture, brushMaterialInstance);
		}
	}



	//UE_LOG(LogTemp, Warning, TEXT("%f, %f"), collisionUV.X, collisionUV.Y);
	FString s = FString::Printf(TEXT("%f, %f"), collisionUV.X, collisionUV.Y);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, s, true, FVector2D(1.5f));
	//collisionUVs.Add(collisionUV);


	if (marker)
	{
		prevCollisionUV = collisionUV;

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

