// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleRoomThreeMorse.generated.h"

UCLASS()
class SPACEESCAPE_API APuzzleRoomThreeMorse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleRoomThreeMorse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* sceneComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* buttonBodyComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* buttonComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* leverBodyComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* leverComp;
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* screenComp;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* leverCollision;
	UPROPERTY(EditDefaultsOnly)
	class UGrabComponent* grabComp;

	void LeverShouldBeZero();
	void ControlByPlayerHand();
	UFUNCTION()
	void ChangeIsGrabed();

private:

	UPROPERTY()
	class AEscapePlayer* player;
	bool bIsGrabed;
};
