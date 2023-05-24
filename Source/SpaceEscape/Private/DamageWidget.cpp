// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageWidget.h"

void UDamageWidget::PlayDamageAnimation()
{
	PlayAnimation(damageAnim, 0, 1, EUMGSequencePlayMode::Forward);
}
