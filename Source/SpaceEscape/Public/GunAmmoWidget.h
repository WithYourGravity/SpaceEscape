// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GunAmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UGunAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo10;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo9;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo8;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo7;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo6;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo5;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo4;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo3;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* img_ammo1;
;
	
};
