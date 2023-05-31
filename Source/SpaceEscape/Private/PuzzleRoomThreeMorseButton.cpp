// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorseButton.h"

#include "Components/AudioComponent.h"

APuzzleRoomThreeMorseButton::APuzzleRoomThreeMorseButton()
{
	Tags.Add(FName("Sense"));
	buttonComp->ComponentTags.Add(FName("Sense.R3"));
}

void APuzzleRoomThreeMorseButton::ButtonTriggered()
{
	Super::ButtonTriggered();
	pushedTime = 0;
	GetWorldTimerManager().SetTimer(addTimeHandle, this, &APuzzleRoomThreeMorseButton::AddTimeStart, 0.1f, true);
}

void APuzzleRoomThreeMorseButton::ButtonEnded()
{
	Super::ButtonEnded();
	GetWorldTimerManager().ClearTimer(addTimeHandle);
	morseButtonDele.Execute(pushedTime);
}

void APuzzleRoomThreeMorseButton::AddTimeStart()
{
	pushedTime += 0.1;
}
