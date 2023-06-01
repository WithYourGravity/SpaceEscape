// Fill out your copyright notice in the Description page of Project Settings.


#include "StageProgressChecker.h"

#include "EscapePlayer.h"
#include "RoomManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStageProgressChecker::AStageProgressChecker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(32.f));
	boxComp->SetRelativeScale3D(FVector(6, 1, 3));
	boxComp->SetCollisionProfileName(FName("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void AStageProgressChecker::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AStageProgressChecker::OnOverlap);
	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));
}

// Called every frame
void AStageProgressChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStageProgressChecker::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bDoOnce)
	{
		return;	
	}

	if (auto player = Cast<AEscapePlayer>(OtherActor))
	{
		bDoOnce = true;
		rm->StageProgressChecker();
	}
}

