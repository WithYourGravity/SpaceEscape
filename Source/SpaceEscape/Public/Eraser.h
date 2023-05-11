// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Eraser.generated.h"

UCLASS()
class SPACEESCAPE_API AEraser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEraser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Eraser")
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Eraser")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Eraser")
	class UGrabComponent* grabComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eraser")
	FLinearColor brushColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eraser")
	float brushSize = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eraser")
	float brushStrength = 1.0f;

	UPROPERTY()
	class AEscapePlayer* player;

	bool bUsing = false;

	UFUNCTION()
	void OnGrabbed();
	UFUNCTION()
	void OnDropped();

	void DetectHitBoard();
};
