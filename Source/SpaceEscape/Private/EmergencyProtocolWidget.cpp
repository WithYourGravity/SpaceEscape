// Fill out your copyright notice in the Description page of Project Settings.


#include "EmergencyProtocolWidget.h"
#include "SpaceEscapeGameModeBase.h"
#include "Components/WidgetSwitcher.h"

void UEmergencyProtocolWidget::NativeConstruct()
{
	Super::NativeConstruct();
	gm = Cast<ASpaceEscapeGameModeBase>(GetWorld()->GetAuthGameMode());
	gm->changeLanguageDele.AddUFunction(this, FName("ChangeProtocolLanguage"));
	ChangeProtocolLanguage(gm->currentLanguageSetting);
}

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
