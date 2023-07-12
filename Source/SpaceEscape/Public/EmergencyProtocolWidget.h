// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceEscapeGameModeBase.h"
#include "EmergencyProtocolWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UEmergencyProtocolWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UWidgetSwitcher* switcherForLanguage;

	UPROPERTY(EditDefaultsOnly)
	class ASpaceEscapeGameModeBase* gm;

	void ChangeProtocolLanguage(ELanguageSettings language);
};
