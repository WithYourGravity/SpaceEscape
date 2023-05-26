// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawableFloor.generated.h"

UCLASS()
class SPACEESCAPE_API ADrawableFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawableFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Floor")
	class UStaticMeshComponent* floorMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Clipboard")
	class UMaterialInstance* pageMaterialInst;

	void OnPaintVisualTraceLine(class AActor* brush, const FHitResult& hitInfo);

private:
	int32 renderTextureSizeX = 1024;
	int32 renderTextureSizeY = 1024;

	FVector2D prevCollisionUV;

	UPROPERTY()
		class UTextureRenderTarget2D* renderToTexture;

	UPROPERTY()
		class UMaterialInstanceDynamic* brushMaterialInstance;
	UPROPERTY()
		class UMaterialInstanceDynamic* eraseBrushMaterialInstance;
	UPROPERTY()
		class UMaterialInterface* paintBrushMaterialInterface;
	UPROPERTY()
		class UMaterialInterface* eraseBrushMaterialInterface;
};
