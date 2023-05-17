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
	class UProgressBar* curHPPercent;

	UFUNCTION(BlueprintCallable)
	void PrintCurrentHP();
	UFUNCTION(BlueprintCallable)
	void PrintCurrentHPPercent();

private:
	UPROPERTY()
	class AEscapePlayer* player;
};
