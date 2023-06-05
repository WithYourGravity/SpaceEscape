// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableVent.h"
#include "GrabComponent.h"
#include "Components/BoxComponent.h"

// �κ� �������� ���
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
	cameOutScrewCount = 0;
	//���簡 �� ������ �� Grab�� �� �ִ�.
	boxComp->SetSimulatePhysics(true);
	grabComp->bSimulateOnDrop = true;
	grabComp->grabType = EGrabType::FREE;
}

