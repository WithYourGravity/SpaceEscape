// Fill out your copyright notice in the Description page of Project Settings.


#include "interactiveObj.h"
#include "Components/BoxComponent.h"
#include "GrabComponent.h"

// Sets default values
AinteractiveObj::AinteractiveObj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetSimulatePhysics(true);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::FREE;
}

// Called when the game starts or when spawned
void AinteractiveObj::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AinteractiveObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

