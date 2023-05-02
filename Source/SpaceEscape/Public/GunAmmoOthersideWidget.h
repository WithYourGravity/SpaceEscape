// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GunAmmoOthersideWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UGunAmmoOthersideWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo10o;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo9o;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo8o;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo7o;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo6o;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo5o;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo4o;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo3o;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo2o;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* img_ammo1o;
	
};
