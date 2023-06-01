// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorseLever.h"
#include "Kismet/GameplayStatics.h"

APuzzleRoomThreeMorseLever::APuzzleRoomThreeMorseLever()
{
	ConstructorHelpers::FObjectFinder<USoundWave>tempSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/morseLeverSound.morseLeverSound'"));
	if (tempSound.Succeeded())
	{
		leverSound = tempSound.Object;
	}

	Tags.Add(FName("Sense"));
	leverComp->ComponentTags.Add(FName("Sense.R3"));
	leverComp->ComponentTags.Add(FName("Sense.R4"));
}

void APuzzleRoomThreeMorseLever::LeverTriggered()
{
	Super::LeverTriggered();
	morseLeverDele.Execute();

	UGameplayStatics::PlaySound2D(this, leverSound);
}
