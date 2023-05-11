// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FClearDele);

UCLASS()
class SPACEESCAPE_API APuzzleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FClearDele puzzleClearDele;

	void ReportClear();
private:
	bool bReportClearOnce;
};
