// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorseLever.h"

APuzzleRoomThreeMorseLever::APuzzleRoomThreeMorseLever()
{
	Tags.Add(FName("Sense"));
	leverComp->ComponentTags.Add(FName("Sense.R3"));
}

void APuzzleRoomThreeMorseLever::LeverTriggered()
{
	Super::LeverTriggered();
	morseLeverDele.Execute();
}
