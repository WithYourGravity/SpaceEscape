// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceEscapeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API ASpaceEscapeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	int32 minute = 0;
	int32 second = 0;

	FTimerHandle countTimer;

	UPROPERTY()
	class AEscapePlayer* player;

	void CountPlayTime();
	void StopPlayTime();
};
