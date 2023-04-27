// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityChange.h"
#include "EscapePlayer.h"
#include "Components/BoxComponent.h"

// Sets default values
AGravityChange::AGravityChange()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGravityChange::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AGravityChange::OnOverlap);
}

// Called every frame
void AGravityChange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGravityChange::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<AEscapePlayer>(OtherActor);

	if (player != nullptr)
	{
		player->bIsZeroGravity = !(player->bIsZeroGravity);

		player->SetActorLocation(player->GetActorLocation() + FVector(0, 100, 1000), true, nullptr, ETeleportType::None);

		AWorldSettings* worldSettings = GetWorldSettings();
		if (player->bIsZeroGravity)
		{
			worldSettings->bGlobalGravitySet = true;
			worldSettings->GlobalGravityZ = -10.0f;
		}
		else
		{
			worldSettings->bGlobalGravitySet = true;
			worldSettings->GlobalGravityZ = -980.0f;
		}
	}
}
