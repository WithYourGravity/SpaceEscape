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
	virtual void BeginPlay() override;

 	// 배터리 장착 가능 범위 콜리전
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;
	// 배터리가 고정될 위치 컴포넌트
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* targerPosition;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* cableMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* batterySound;
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WhenScrewFinished();

private:

	UPROPERTY()
	class AMovableBattCover* myCover;

	bool bIsScrewFinished;
};
