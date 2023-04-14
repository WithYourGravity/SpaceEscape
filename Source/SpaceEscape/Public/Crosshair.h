// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crosshair.generated.h"

UCLASS()
class SPACEESCAPE_API ACrosshair : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrosshair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	class USceneComponent* defaultSceneComp;

	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	class UStaticMeshComponent* crosshairComp;
};
