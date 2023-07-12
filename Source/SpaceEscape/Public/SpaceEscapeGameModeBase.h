// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceEscapeGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FChangeLanguageDele, ELanguageSettings language);

UENUM(BlueprintType)
enum class ELanguageSettings : uint8
{
	ENGLISH,
	KOREAN
};

UCLASS()
class SPACEESCAPE_API ASpaceEscapeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	int32 minute = 0;
	int32 second = 0;

	FTimerHandle countTimer;
	ELanguageSettings currentLanguageSetting = ELanguageSettings::ENGLISH;
	FChangeLanguageDele changeLanguageDele;

	UPROPERTY()
	class AEscapePlayer* player;
	UPROPERTY()
	class ARoomManager* rm;

	void CountPlayTime();
	void StopPlayTime();
	void StartPlayTime();
	void SetStage(int stage);
	void ChangeLanguageSetting(ELanguageSettings language);
};
