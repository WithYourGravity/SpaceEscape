// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorLight.generated.h"

UENUM()
enum class ColorList : uint8
{
	Red,
	Yellow,
	Green
};

UCLASS()
class SPACEESCAPE_API ADoorLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AddSolvedPuzzleCountForDoorLight();
	void ChangeColorByCount();

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;

private:
	int solvedPuzzleCountForDoorLight;
};
