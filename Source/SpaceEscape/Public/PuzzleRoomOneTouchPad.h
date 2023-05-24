// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "PuzzleRoomOneTouchPad.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API APuzzleRoomOneTouchPad : public APuzzleBase
{
	GENERATED_BODY()

public:
	APuzzleRoomOneTouchPad();
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* screenWidgetComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp0;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp1;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp2;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp3;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp4;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp5;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp6;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp7;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp8;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyComp9;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyCompEnt;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* touchKeyCompDel;
	UPROPERTY()
	class UPuzzleRoomOneTouchPadPanelWidget* rmOnePanelWidget;
	UPROPERTY()
	class UPuzzleRoomTwoTouchPadPanelWidget* rmTwoPanelWidget;

	UFUNCTION()
	void TouchPadOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WhenBatteryOn();

	void CheckPassword();
	void TouchPadInput(FString number);

private:

	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* hapticFeedback;
	UPROPERTY()
	class AGravityChange* gc;
	UPROPERTY()
	class ARoomManager* rm;
	UPROPERTY()
	class APuzzleRoomOneBattery* battery;

	FString answer;
	bool bCanTouch = true;
	float touchDelay = 0.6;
};
