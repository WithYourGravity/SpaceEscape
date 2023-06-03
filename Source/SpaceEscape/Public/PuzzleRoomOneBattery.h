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

 	// ���͸� ���� ���� ���� �ݸ���
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;
	// ���͸��� ������ ��ġ ������Ʈ
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
