// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityChange.h"

#include "DoorButton.h"
#include "EscapePlayer.h"
#include "RoomManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGravityChange::AGravityChange()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	//SetRootComponent(boxComp);

	//meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	//meshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGravityChange::BeginPlay()
{
	Super::BeginPlay();

	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &AGravityChange::OnOverlap);

	//player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	worldSettings = GetWorldSettings();
	worldSettings->bGlobalGravitySet = true;

	roomManager = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoomManager::StaticClass()));

	doorButton = Cast<ADoorButton>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoorButton::StaticClass()));

	if (doorButton)
	{
		doorButton->openDoorDele.AddUFunction(this, FName("ChangeStageTwoGravity"));
	}
}

// Called every frame
void AGravityChange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGravityChange::ChangeGravity(bool bCurGravity)
{
	bIsZeroGravity = !bCurGravity;

	if (bIsZeroGravity)
	{
		worldSettings->GlobalGravityZ = -10.0f;
	}
	else
	{
		worldSettings->GlobalGravityZ = -980.0f;
	}
}

void AGravityChange::ChangeStageTwoGravity()
{
	//if (roomManager->playingStage)

	ChangeGravity(bIsZeroGravity);
}

//void AGravityChange::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	auto player = Cast<AEscapePlayer>(OtherActor);
//
//	if (player != nullptr)
//	{
//		player->bIsZeroGravity = !(player->bIsZeroGravity);
//
//		player->SetActorLocation(player->GetActorLocation() + FVector(0, 100, 1000), true, nullptr, ETeleportType::None);
//
//		AWorldSettings* worldSettings = GetWorldSettings();
//		if (player->bIsZeroGravity)
//		{
//			worldSettings->bGlobalGravitySet = true;
//			worldSettings->GlobalGravityZ = -10.0f;
//		}
//		else
//		{
//			worldSettings->bGlobalGravitySet = true;
//			worldSettings->GlobalGravityZ = -980.0f;
//		}
//	}
//}
