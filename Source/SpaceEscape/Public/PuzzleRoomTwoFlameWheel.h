// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleRoomTwoFlameWheel.generated.h"

UCLASS()
class SPACEESCAPE_API APuzzleRoomTwoFlameWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleRoomTwoFlameWheel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* sceneComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly)
	class UGrabComponent* grabComp;
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* soundComp;

	UFUNCTION()
	void WhenPlayerGrab();
	UFUNCTION()
	void WhenPlayerUnGrab();

	void ControlByPlayerHand();

	UPROPERTY(EditAnywhere)
	int arrayIndex = 0;

private:

	UPROPERTY()
	class AEscapePlayer* player;
	UPROPERTY()
	TArray<class AActor*> flameArray;
	UPROPERTY()
	class APuzzleRoomTwoFlame* selectedFlame;

	bool bIsGrabed;
	bool bRecordOnce;
	bool bsoundPlayOnce;
	FVector startVector;
	FVector playerDirection;
};
