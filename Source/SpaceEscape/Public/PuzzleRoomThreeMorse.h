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

	UFUNCTION()
	void addToTempString(float second);
	UFUNCTION()
	void Enter();
	UFUNCTION()
	void ForEndingRanking();

	void CheckRightOrWrong();
	void setScreenText(FString string);
	void EmptyScreenString();
	char Translater(FString code);
	FString GetScreenString();

private:

	UPROPERTY()
	class ARoomManager* rm;
	bool bAnswerOnce;
	// ��ũ���� ����� ���ڿ�
	FString screenString;
	// �𽺹�ư���� �Էµ� 0�� 1�� ���� ���ڿ�
	FString tempString;
	// �� ��ȣ�� �ص��� ��
	TMap<FString, char> morse = {
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
