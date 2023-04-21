// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeverBase.h"
#include "PuzzleRoomThreeMorseLever.generated.h"

DECLARE_DELEGATE(FMorseLeverDele);

UCLASS()
class SPACEESCAPE_API APuzzleRoomThreeMorseLever : public ALeverBase
{
	GENERATED_BODY()

public:
	virtual void LeverTriggered() override;
	FMorseLeverDele morseLeverDele;
};
