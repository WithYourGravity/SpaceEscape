// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "PuzzleRoomThreeMorse.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API APuzzleRoomThreeMorse : public APuzzleBase
{
	GENERATED_BODY()

public:
	APuzzleRoomThreeMorse();
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* screenComp;
	UPROPERTY(EditDefaultsOnly)
	class UPuzzleRoomThreeMorseScreenWidget* screenWidget;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	UFUNCTION()
	void addToTempString(const float second);
	UFUNCTION()
	void Enter();
	UFUNCTION()
	void ForEndingRanking();
	UFUNCTION()
	void WhenShipSticked();
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CheckRightOrWrong();
	void setScreenText(const FString& string);
	void EmptyScreenString();
	char Translater(const FString& code);
	FString GetScreenString();

private:

	UPROPERTY()
	class ARoomManager* rm;
	UPROPERTY()
	class ASpaceShip* ship;
	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* hapticFeedback;

	bool bAnswerOnce;
	bool bIsSticked;
	// 스크린에 출력할 문자열
	FString screenString;
	// 모스버튼으로 입력된 0과 1을 담을 문자열
	FString tempString;
	// 모스 정답
	const FString morseAnswer = "EARTH";
	// 모스 부호를 해독할 맵
	const TMap<FString, char> morse = {
		{"01", 'A'},
		{"1000", 'B'},
		{"1010", 'C'},
		{"100", 'D'},
		{"0", 'E'},
		{"0010", 'F'},
		{"110", 'G'},
		{"0000", 'H'},
		{"00", 'I'},
		{"0111", 'J'},
		{"101", 'K'},
		{"0100", 'L'},
		{"11", 'M'},
		{"10", 'N'},
		{"111", 'O'},
		{"0110", 'P'},
		{"1101", 'Q'},
		{"010", 'R'},
		{"000", 'S'},
		{"1", 'T'},
		{"001", 'U'},
		{"0001", 'V'},
		{"011", 'W'},
		{"1001", 'X'},
		{"1011", 'Y'},
		{"1100", 'Z'},
	};
};
