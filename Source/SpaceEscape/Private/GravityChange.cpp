// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityChange.h"

// Sets default values
AGravityChange::AGravityChange()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGravityChange::BeginPlay()
{
	Super::BeginPlay();

	worldSettings = GetWorldSettings();
	worldSettings->bGlobalGravitySet = true;
	worldSettings->GlobalGravityZ = -980.0f;
}

// Called every frame
void AGravityChange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGravityChange::ChangeGravity(bool bCurGravity)
{
	bIsZeroGravity = bCurGravity;

	if (bIsZeroGravity)
	{
		worldSettings->GlobalGravityZ = -10.0f;
	}
	else
	{
		worldSettings->GlobalGravityZ = -980.0f;
	}
}
