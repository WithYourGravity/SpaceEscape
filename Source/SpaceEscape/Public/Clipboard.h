// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Clipboard.generated.h"

UCLASS()
class SPACEESCAPE_API AClipboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClipboard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Clipboard")
	class UStaticMeshComponent* boardMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Clipboard")
	class UStaticMeshComponent* pageMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* grabComp;

	void OnPaintVisualTraceLine(class APencil* brush, const FHitResult& hitInfo);

private:
	UPROPERTY()
	class UMaterialInstance* pageMaterialInst;

	int32 renderTextureSizeX = 1024;
	int32 renderTextureSizeY = 1024;

	UPROPERTY()
	class UTextureRenderTarget2D* renderToTexture;

	UPROPERTY()
	class UMaterialInstanceDynamic* brushMaterialInstance;
	UPROPERTY()
	class UMaterialInterface* paintBrushMaterialInterface;

	UPROPERTY()
	class APencil* pencil;
};
