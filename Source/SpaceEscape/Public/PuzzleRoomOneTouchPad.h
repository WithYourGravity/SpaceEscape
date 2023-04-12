// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "PuzzleRoomOneTouchPad.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API APuzzleRoomOneTouchPad : public APuzzleBase
{
	GENERATED_BODY()

public:
	APuzzleRoomOneTouchPad();
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* screenWidgetComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;
	//UPROPERTY(EditDefaultsOnly)
	//class UBoxComponent* boxComp;

};
