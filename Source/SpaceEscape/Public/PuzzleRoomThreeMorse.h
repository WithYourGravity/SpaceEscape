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
	void Enter();
	FString Translater(FString code);

private:
	FString tempString;
};
