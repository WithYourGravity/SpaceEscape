// Fill out your copyright notice in the Description page of Project Settings.


#include "EmergencyProtocolWidget.h"
#include "SpaceEscapeGameModeBase.h"
#include "Components/WidgetSwitcher.h"

// ��� ������ ���� ��� �������� �� �ٲ��ִ� �Լ�
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
