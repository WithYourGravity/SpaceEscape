// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchPadOverlap.h"

#include "DialogueWidget.h"
#include "EnemyDoorOverlap.h"
#include "EscapePlayer.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"

// Sets default values
ATouchPadOverlap::ATouchPadOverlap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(100.0f);
}

// Called when the game starts or when spawned
void ATouchPadOverlap::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATouchPadOverlap::OnBeginOverlap);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ATouchPadOverlap::OnEndOverlap);
}

// Called every frame
void ATouchPadOverlap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATouchPadOverlap::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<AEscapePlayer>(OtherActor);
	if (player && !bIsOverlapDoor)
	{
		bIsOverlapDoor = true;

		// Show Dialogue
		player->dialogueUI->text_dialogue->SetText(FText::FromString(TEXT("아이씨..여기 들어오면서 ID 카드 떨어뜨린 것 같아")));
		player->dialogueWidgetComp->SetVisibility(true);

		player->HiddenDialogue();
	}
}

void ATouchPadOverlap::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

