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

	UFUNCTION()
	void GoToMain();
	UFUNCTION()
	void DeleteName();
	UFUNCTION()
	void EnterName();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name3;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time3;
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

	UPROPERTY()
	TArray<class UTextBlock*> nameBlockArray;
	UPROPERTY()
	TArray<class UTextBlock*> timeBlockArray;
private:

	UPROPERTY()
	class APuzzleRoomThreeMorse* morse;
	UPROPERTY()
	class ARanking* rankingActor;
};
