// Fill out your copyright notice in the Description page of Project Settings.


#include "EmergencyProtocolWidget.h"
#include "SpaceEscapeGameModeBase.h"
#include "Components/WidgetSwitcher.h"

// 언어 설정에 따라 비상 프로토콜 언어를 바꿔주는 함수
void UEmergencyProtocolWidget::ChangeProtocolLanguage(ELanguageSettings language)
{
	switch (language)
	{
	case ELanguageSettings::ENGLISH:
		switcherForLanguage->SetActiveWidgetIndex(0);
		break;
	case ELanguageSettings::KOREAN:
		switcherForLanguage->SetActiveWidgetIndex(1);
		break;
	}
}
