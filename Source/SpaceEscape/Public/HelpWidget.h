// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HelpWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UHelpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UWidgetSwitcher* languageSwitcher;

	UPROPERTY(EditDefaultsOnly)
	class ASpaceEscapeGameModeBase* gm;

	UFUNCTION()
	void ChangeLanguage(ELanguageSettings language);
};
