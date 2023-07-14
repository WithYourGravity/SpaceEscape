// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDoorOverlap.h"
#include "DialogueWidget.h"
#include "EnemyFSM.h"
#include "EscapePlayer.h"
#include "ResearcherEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceEscapeGameModeBase.h"

// Sets default values
AEnemyDoorOverlap::AEnemyDoorOverlap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(100.0f);
}

// Called when the game starts or when spawned
void AEnemyDoorOverlap::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyDoorOverlap::OnBeginOverlap);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &AEnemyDoorOverlap::OnEndOverlap);

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	gm = Cast<ASpaceEscapeGameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AEnemyDoorOverlap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyDoorOverlap::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto enemy = Cast<AResearcherEnemy>(OtherActor);
	if (enemy && enemy->enemyFSM)
	{
		enemy->enemyFSM->bIsOverlapDoor = true;

		if (!bIsOverlapDoor)
		{
			bIsOverlapDoor = true;

			// Show Dialogue
			if(gm->currentLanguageSetting == ELanguageSettings::KOREAN)
			{
				player->dialogueUI->text_dialogue->SetText(FText::FromString(TEXT("여기서 나가야해 당장")));
			}
			else if(gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
			{
				player->dialogueUI->text_dialogue->SetText(FText::FromString(TEXT("I should get out of here right now")));
			}
			player->ShowDialogue();
		}
	}
}

void AEnemyDoorOverlap::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto enemy = Cast<AResearcherEnemy>(OtherActor);
	if (enemy && enemy->enemyFSM)
	{
		enemy->enemyFSM->bIsOverlapDoor = false;
	}
}
