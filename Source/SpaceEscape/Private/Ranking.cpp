// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranking.h"

#include "Components/WidgetComponent.h"

// Sets default values
ARanking::ARanking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widgetComp"));
	widgetComp->SetupAttachment(RootComponent);
	widgetComp->SetDrawSize(FVector2D(1920, 1080));
	ConstructorHelpers::FClassFinder<UUserWidget>tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_RankingWidget.WBP_RankingWidget_C'"));
	if (tempWidget.Succeeded())
	{
		widgetComp->SetWidgetClass(tempWidget.Class);
	}
}

// Called when the game starts or when spawned
void ARanking::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARanking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

