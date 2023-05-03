// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pencil.generated.h"

UCLASS()
class SPACEESCAPE_API APencil : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APencil();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Pencil")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Pencil")
	class UArrowComponent* arrowComp;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* grabComp;

	UFUNCTION()
	void OnGrabbed();
	UFUNCTION()
	void OnDropped();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pencil")
	FLinearColor brushColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pencil")
	float brushSize = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pencil")
	float brushStrength = 1.0f;

	bool bUsing = false;

	void DetectHitBoard();
};
