// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UMainTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* textBlock_Title;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_GameStart;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Exit;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Credit;

	UFUNCTION()
	void GameStart();
	UFUNCTION()
	void GameExit();
	UFUNCTION()
	void ShowCredit();
};
