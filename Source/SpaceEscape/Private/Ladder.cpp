// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "GrabComponent.h"
#include "Components/BoxComponent.h"

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

	grabComp1 = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp1"));
	grabComp1->SetupAttachment(meshComp);

	grabComp2 = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp2"));
	grabComp2->SetupAttachment(meshComp);

	grabComp3 = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp3"));
	grabComp3->SetupAttachment(meshComp);

	grabComp4 = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp4"));
	grabComp4->SetupAttachment(meshComp);

	grabComp5 = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp5"));
	grabComp5->SetupAttachment(meshComp);

	grabComp6 = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp6"));
	grabComp6->SetupAttachment(meshComp);

	grabComp7 = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp7"));
	grabComp7->SetupAttachment(meshComp);
}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

