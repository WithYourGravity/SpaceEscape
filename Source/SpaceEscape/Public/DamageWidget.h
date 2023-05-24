// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* damageAnim;

	void PlayDamageAnimation();
};
