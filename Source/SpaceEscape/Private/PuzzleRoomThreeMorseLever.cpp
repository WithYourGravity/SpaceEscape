// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorseLever.h"

void APuzzleRoomThreeMorseLever::LeverTriggered()
{
	Super::LeverTriggered();
	morseLeverDele.Execute();
}
