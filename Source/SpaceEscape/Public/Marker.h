// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Marker.generated.h"

UCLASS()
class SPACEESCAPE_API AMarker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Marker")
	class UStaticMeshComponent* rootCylinderMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Marker")
	class UStaticMeshComponent* cylinderMeshComp1;

	UPROPERTY(EditDefaultsOnly, Category = "Marker")
	class UStaticMeshComponent* cylinderMeshComp2;

	UPROPERTY(EditDefaultsOnly, Category = "Marker")
	class UStaticMeshComponent* cylinderMeshComp3;

	UPROPERTY(EditDefaultsOnly, Category = "Marker")
	class UStaticMeshComponent* tipMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* grabComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker")
	FLinearColor brushColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker")
	float brushSize = 0.02f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker")
	float brushStrength = 1.0f;

	bool bUsing = false;

	UPROPERTY()
	class AEscapePlayer* player;

	UPROPERTY()
	class UMaterialInstance* colorMaterialInst;

	UFUNCTION()
	void OnGrabbed();
	UFUNCTION()
	void OnDropped();

	void DetectHitBoard();
};
