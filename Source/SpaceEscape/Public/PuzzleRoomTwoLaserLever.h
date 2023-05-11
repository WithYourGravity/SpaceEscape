// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeverBase.h"
#include "PuzzleRoomTwoLaserLever.generated.h"

DECLARE_DELEGATE(FLaserLeverDele);

UCLASS()
class SPACEESCAPE_API APuzzleRoomTwoLaserLever : public ALeverBase
{
	GENERATED_BODY()
public:
	virtual void LeverTriggered() override;
	FLaserLeverDele laserLaverDele;
};
