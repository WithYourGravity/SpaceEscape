// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "PuzzleRoomOneBattery.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API APuzzleRoomOneBattery : public APuzzleBase
{
	GENERATED_BODY()

public:
	APuzzleRoomOneBattery();
	//virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;
	/*
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void SetBattery();

private:
	UPROPERTY()
	class ABattery* battery;
	bool bCanSetBattery;
	*/
};
