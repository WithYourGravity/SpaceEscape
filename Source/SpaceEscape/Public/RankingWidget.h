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
	void PlayClickSound();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name3;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name4;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name5;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name6;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name7;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name8;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Name9;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time3;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time4;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time5;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time6;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time7;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time8;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* text_Time9;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Exit;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Delete;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_DeleteEng;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Enter;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_EnterEng;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UWidgetSwitcher* switcher;

	UPROPERTY()
	TArray<class UTextBlock*> nameBlockArray;
	UPROPERTY()
	TArray<class UTextBlock*> timeBlockArray;
private:

	UPROPERTY()
	class APuzzleRoomThreeMorse* morse;
	UPROPERTY()
	class ARanking* rankingActor;
	UPROPERTY()
	class USoundBase* clickSound;
};
