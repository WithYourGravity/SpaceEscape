// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovableCover.h"
#include "MovableVent.generated.h"

UCLASS()
class SPACEESCAPE_API AMovableVent : public AMovableCover
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovableVent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//로봇 지나가는 통로가 있는 Vent

	virtual void FallingCover() override;

};
