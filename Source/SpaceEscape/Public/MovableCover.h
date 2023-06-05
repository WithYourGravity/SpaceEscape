// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableCover.generated.h"

UCLASS()
class SPACEESCAPE_API AMovableCover : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovableCover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	class UGrabComponent* grabComp;
	
	virtual void FallingCover();

	int32 cameOutScrewCount = 0;
};
