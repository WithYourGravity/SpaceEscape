// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ButtonBase.h"
#include "PuzzleRoomThreeMorseButton.generated.h"

DECLARE_DELEGATE_OneParam(FMorseButtonDele, float pushedTime);

UCLASS()
class SPACEESCAPE_API APuzzleRoomThreeMorseButton : public AButtonBase
{
	GENERATED_BODY()

public:
	APuzzleRoomThreeMorseButton();

	virtual void ButtonTriggered() override;
	virtual void ButtonEnded() override;
	void AddTimeStart();
	FMorseButtonDele morseButtonDele;
private:
	float pushedTime;
	FTimerHandle addTimeHandle;
};
