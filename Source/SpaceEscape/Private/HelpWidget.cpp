// Fill out your copyright notice in the Description page of Project Settings.


#include "HelpWidget.h"
#include "SpaceEscapeGameModeBase.h"
#include "Components/WidgetSwitcher.h"

void UHelpWidget::ChangeLanguage(ELanguageSettings language)
{
	switch (language)
	{
	case ELanguageSettings::ENGLISH:
		languageSwitcher->SetActiveWidgetIndex(1);
		break;
	case ELanguageSettings::KOREAN:
		languageSwitcher->SetActiveWidgetIndex(0);
		break;
	}
}
