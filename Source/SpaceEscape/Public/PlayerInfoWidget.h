// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (BindWidget))
	class UTextBlock* curHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (BindWidget))
	class UImage* curHPPercentImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (BindWidget))
	class UTextBlock* curMinute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (BindWidget))
	class UTextBlock* curSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	class UMaterialInstance* hpMI;

	UFUNCTION(BlueprintCallable)
	void PrintCurrentHPPercent();
	void PrintCurrentPlayTime();

private:
	UPROPERTY()
	class AEscapePlayer* player;

	UPROPERTY()
	class ASpaceEscapeGameModeBase* gm;

	UPROPERTY()
	class UMaterialInstanceDynamic* hpDMI;
};
