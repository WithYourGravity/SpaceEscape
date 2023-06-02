// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableVent.h"

// 로봇 지나가는 통로
AMovableVent::AMovableVent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> vMesh(TEXT("/Script/Engine.StaticMesh'/Game/Yeni/Assets/ModularScifiProps/Meshes/SM_Vent_A.SM_Vent_A'"));
	if (vMesh.Succeeded())
	{
		meshComp->SetStaticMesh(vMesh.Object);
	}
	
}

// Called when the game starts or when spawned
void AMovableVent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMovableVent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovableVent::FallingCover()
{
	Super::FallingCover();
}

//void AMovableVent::FallingVent()
//{
//	if (cameOutScrewCount < 4 )
//	{
//		UE_LOG(LogTemp, Warning, TEXT("isEnoughRotated"))
//		cameOutScrewCount++;
//		UE_LOG(LogTemp, Warning, TEXT("cameOutScrew Count : %d"), cameOutScrewCount)
//		boxComp->SetSimulatePhysics(false);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("cameOutScrew Count222 : %d"), cameOutScrewCount)
//		boxComp->SetSimulatePhysics(true);
//	}
//}

