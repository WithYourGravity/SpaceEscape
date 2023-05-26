// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SpaceEscapeSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API USpaceEscapeSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString slotName;
	UPROPERTY()
	int32 saveIndex;
	UPROPERTY()
	TMap<FString, int32> rankerMap;
};
