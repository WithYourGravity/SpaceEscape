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

	//�κ� �������� ��ΰ� �ִ� Vent

	virtual void FallingCover() override;

};
