// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "PuzzleRoomTwoLaserWheel.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API APuzzleRoomTwoLaserWheel : public APuzzleBase
{
	GENERATED_BODY()

public:

	APuzzleRoomTwoLaserWheel();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	//UPROPERTY(EditDefaultsOnly)
	//class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly)
	class UGrabComponent* grabComp;

	UFUNCTION()
	void ChangeLaserIndex();
	UFUNCTION()
	void ChangeIsGrabed();
	void ControlByPlayerHand();

private:

	UPROPERTY()
	class AEscapePlayer* player;
	UPROPERTY()
	TArray<class AActor*> laserArray;
	UPROPERTY()
	class APuzzleRoomTwoLaserLever* laserLever;
	UPROPERTY()
	class APuzzleRoomTwoLaser* currentLaser;
	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* hapticFeedback;

	int arrayIndex;
	bool bIsGrabed;
	bool bRecordOnce;
	FVector startVector;
};
