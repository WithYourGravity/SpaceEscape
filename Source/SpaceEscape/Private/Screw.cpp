// Fill out your copyright notice in the Description page of Project Settings.


#include "Screw.h"
#include "Components/BoxComponent.h"
#include "Nail.h"

AScrew::AScrew()
{ 	
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SCREW Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(20, 20, 50));
	boxComp->SetRelativeRotation(FRotator(0, 90, 0));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewMesh"));
	meshComp->SetupAttachment(RootComponent);

}

void AScrew::BeginPlay()
{
	Super::BeginPlay();

	//드라이버가 손과 닿는다
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AScrew::AttachScrewToHand);


	
}

void AScrew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	


}

void AScrew::AttachScrewToHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//드라이버가 손에 붙는다

}


