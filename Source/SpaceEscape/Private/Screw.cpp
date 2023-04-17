// Fill out your copyright notice in the Description page of Project Settings.


#include "Screw.h"
#include "Components/BoxComponent.h"
#include "Nail.h"
#include "EscapePlayer.h"

AScrew::AScrew()
{ 	
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SCREW Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(20, 20, 50));
	boxComp->SetRelativeRotation(FRotator(0, 90, 0));
	boxComp->SetSimulatePhysics(true);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewMesh"));
	meshComp->SetupAttachment(RootComponent);

	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController());

}

void AScrew::BeginPlay()
{
	Super::BeginPlay();


	
}

void AScrew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	


}

void AScrew::AttachedByHand()
{
	if(player->bIsGrabbed == false) return;
	
	//드라이버가 손에 붙으면
	

}




