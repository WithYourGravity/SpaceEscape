// Fill out your copyright notice in the Description page of Project Settings.


#include "MainTitle.h"

#include "Components/WidgetComponent.h"

// Sets default values
AMainTitle::AMainTitle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widgetComp"));
	widgetComp->SetupAttachment(RootComponent);
	widgetComp->SetRelativeScale3D(FVector(0.15f));
	widgetComp->SetDrawSize(FVector2D(1920, 1080));
	ConstructorHelpers::FClassFinder<UUserWidget>tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_MainTitleWidget.WBP_MainTitleWidget_C'"));
    if (tempWidget.Succeeded())
    {
		widgetComp->SetWidgetClass(tempWidget.Class);
    }

	ConstructorHelpers::FObjectFinder<USoundBase>tempClickSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/UIClick.UIClick'"));
	if (tempClickSound.Succeeded())
	{
		clickSound = tempClickSound.Object;
	}
}

// Called when the game starts or when spawned
void AMainTitle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainTitle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

