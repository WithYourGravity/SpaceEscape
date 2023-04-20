// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrewDriver.h"
#include "Components/BoxComponent.h"
#include "Screw.h"
#include "EscapePlayer.h"
#include "MotionControllerComponent.h"

// Sets default values
AScrewDriver::AScrewDriver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	handleBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ScrewHandleCollision"));
	SetRootComponent(handleBoxComp);
	handleBoxComp->SetRelativeLocation(FVector(0, 0, 30));
	handleBoxComp->SetBoxExtent(FVector(40, 40, 20));
	handleBoxComp->SetSimulatePhysics(true);
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Screw Collision"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetBoxExtent(FVector(5, 5, 50));
	boxComp->SetRelativeRotation(FRotator(0, 90, 90));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewMesh"));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AScrewDriver::BeginPlay()
{
	Super::BeginPlay();

	initRot = GetActorRotation().Roll;
	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController());
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AScrewDriver::AttachNailtoScrew);
	initScrew();
}

// Called every frame
void AScrewDriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScrewDriver::AttachedByHand()
{
	if (player->bIsGrabbed == false) return;

	if (player->bIsGrabbed == true && attachedScrew != nullptr)
	{
		//����̹��� �տ� ������
		SetActorRotation(player->GetActorRotation());
		SetActorLocation(player->rightAim->GetComponentLocation());
	}
}

void AScrewDriver::AttachNailtoScrew(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	attachedScrew = Cast<AScrew>(OtherActor);
	if (attachedScrew != nullptr)
	{
		isAttaching = true;
		attachedScrew->boxComp->SetSimulatePhysics(false);
		attachedScrew->boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//attachedScrew->AttachToComponent(boxComp, FAttachmentTransformRules::KeepWorldTransform);
		attachedScrew->SetActorLocation(attachedScrew->GetActorLocation());
		//auto y = attachedNail->GetActorRotation().Yaw;
		//auto p = attachedNail->GetActorRotation().Pitch;
		//DisableInput(attachedNail->GetActorRotation().Yaw);

		AttachNailProcess();
	}
}

void AScrewDriver::AttachNailProcess()
{
	//���� ȸ���ϴ� ��ŭ ����̹��� �����̴µ�, ���絵 ���� �����̴°�

	//����̹��� ���� ȸ��
	//auto prevScrewRot = GetActorRotation().Roll - 10;

	//������ ȸ����
	//auto nailRot = attachedNail->GetActorRotation().Roll;
	if (isAttaching == true)
	{
		double currentRot = GetActorRotation().Roll;
		if (initRot - currentRot > 10)
		{
			ComingoutNails();
		}
	}

}

void AScrewDriver::ComingoutNails()
{
	//������ ������ ��
	attachedScrew->SetActorLocation(GetActorLocation() + FVector(50, 0, 0));
}


void AScrewDriver::initScrew()
{
	isAttaching = false;
	attachedScrew = nullptr;
	initRot = 0;
}

//����̹��� ���翡 ������ ����̹��� ȸ�� ���� ���缭 ���縦 ȸ����Ų��

