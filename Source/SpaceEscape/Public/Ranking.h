// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ranking.generated.h"

UCLASS()
class SPACEESCAPE_API ARanking : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARanking();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* sceneComp;
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* widgetComp;

	UFUNCTION()
	void CalculateRecord();
	UFUNCTION()
	void SetVisible();
	void LoadSavedData();
	void SaveData();
	void AddToRanking(FString curName);
	void SetRankingDataToWidget();

private:

	UPROPERTY()
	class ASpaceEscapeGameModeBase* gm;
	UPROPERTY()
	class ARoomManager* rm;
	UPROPERTY()
	class URankingWidget* rankingWidget;
	UPROPERTY()
	class AEscapePlayer* player;

	UPROPERTY()
	TMap<FString, int> rankerMap;
	UPROPERTY()
	TArray<int> rankTimeArr;
	UPROPERTY()
	TArray<FString> rankNameArr;

	int curRecord;
	int index;

};
