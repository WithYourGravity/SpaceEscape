// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RankingWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API URankingWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void GoToMain();
	void DeleteName();
	void EnterName();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_NameFirst;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_NameSecond;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_NameThird;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_TimeFirst;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_TimeSecond;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_TimeThird;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Exit;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Delete;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Enter;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UWidgetSwitcher* switcher;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_MorseName;

};
