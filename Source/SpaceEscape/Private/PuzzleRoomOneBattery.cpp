// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneBattery.h"

#include "Battery.h"
#include "GrabComponent.h"
#include "Components/BoxComponent.h"

APuzzleRoomOneBattery::APuzzleRoomOneBattery()
{
	targerPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("Position For Battery"));
	SetRootComponent(targerPosition);
	targerPosition->SetBoxExtent(FVector(8));
	targerPosition->SetCollisionProfileName(FName("NoCollision"));

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Mountable Range"));
	boxComp->SetupAttachment(RootComponent);

	cableMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cableMeshComp"));
	cableMeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempCableMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Wire/SM_WVD_Wires_A.SM_WVD_Wires_A'"));
	if (tempCableMesh.Succeeded())
	{
		cableMeshComp->SetStaticMesh(tempCableMesh.Object);
	}
	cableMeshComp->SetRelativeScale3D(FVector(0.5f));
	cableMeshComp->SetRelativeLocation(FVector(-26.f, -5.f, 21.f));
}

void APuzzleRoomOneBattery::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomOneBattery::BeginOverlap);
}

// ���͸��� ���������� ������ ���������ϰ� �ϴ� �Լ�
void APuzzleRoomOneBattery::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto battery = Cast<ABattery>(OtherActor);
	if (battery)
	{
		battery->grabComp->TryRelease();
		battery->SetActorLocationAndRotation(targerPosition->GetComponentLocation(), FRotator::ZeroRotator);
		battery->meshComp->SetSimulatePhysics(false);
		battery->meshComp->SetCollisionProfileName(FName("NoCollision"));
		boxComp->SetCollisionProfileName(FName("NoCollision"));
		ReportClear();
	}
}