// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoLaserLever.h"

void APuzzleRoomTwoLaserLever::LeverTriggered()
{
	Super::LeverTriggered();
	laserLaverDele.Execute();
}
