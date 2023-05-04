// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleRoomTwoLaser.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API APuzzleRoomTwoLaser : public AActor
{
	GENERATED_BODY()

public:
	APuzzleRoomTwoLaser();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* sceneComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* laserStartMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* laserNiagaraComp;

	void LetsTraceLaser(class UStaticMeshComponent* startPointMeshComp);


};
