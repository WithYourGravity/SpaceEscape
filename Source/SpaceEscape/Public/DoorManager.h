// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorManager.generated.h"

UCLASS()
class SPACEESCAPE_API ADoorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TArray<AActor*> doors;
	UPROPERTY()
	TArray<AActor*> doorBtns;

	class ARoomManager* rm;
	bool bOpened;


	void OpenAllDoors();
	void CloseAllDoors();
};
