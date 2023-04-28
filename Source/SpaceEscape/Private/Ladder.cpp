// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALadder::ALadder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(7, 46, 210));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Ladder/Ladder.Ladder'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		meshComp->SetRelativeRotation(FRotator(0, -90, 0));
		meshComp->SetRelativeScale3D(FVector(0.4f));
	}

	for (int32 i = 0; i < barNum; i++)
	{
		FString compName = FString::Printf(TEXT("grabComp%d"), i + 1);
		auto grabComp = CreateDefaultSubobject<UGrabComponent>(FName(*compName));
		grabComp->SetupAttachment(meshComp);
		grabComp->grabType = EGrabType::CLIMB;
		grabComps.Add(grabComp);
	}
	
}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 i = 0; i < barNum; i++)
	{
		grabComps[i]->onGrabbedDelegate.AddUFunction(this, FName("OnGrabbed"));
		grabComps[i]->onDroppedDelegate.AddUFunction(this, FName("OnDropped"));
	}

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALadder::OnGrabbed()
{
	player->bIsClimbing = true;
}

void ALadder::OnDropped()
{
	if (!(player->bIsGrabbedLeft) && !(player->bIsGrabbedRight))
	{
		player->bIsClimbing = false;	
	}
}

