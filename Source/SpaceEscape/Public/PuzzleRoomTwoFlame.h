// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleRoomTwoFlame.generated.h"

UCLASS()
class SPACEESCAPE_API APuzzleRoomTwoFlame : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleRoomTwoFlame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* sceneComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* pipeTopMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* pipeBottomMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystemComponent* flameComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* sphereCompForTrace;

	void DoTrace();
	void CloseValve(float degree);

private:
	UPROPERTY()
	class ARoomManager* rm;
	UPROPERTY()
	class APuzzleRoomTwoFlameHitPoint* hitPoint;
	float degreeToClose;
	bool bIsClosed;

};
