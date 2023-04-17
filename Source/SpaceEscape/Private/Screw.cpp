// Fill out your copyright notice in the Description page of Project Settings.


#include "Screw.h"
#include "Components/BoxComponent.h"
#include "Nail.h"
#include "EscapePlayer.h"
#include "MotionControllerComponent.h"

AScrew::AScrew()
{ 	
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

void AScrew::BeginPlay()
{
	Super::BeginPlay();

	initRot = GetActorRotation().Roll;
	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController());
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AScrew::AttachNailtoScrew);
	initNail();
}

void AScrew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScrew::AttachedByHand()
{
	if(player->bIsGrabbed == false) return;

	if(player->bIsGrabbed == true && attachedNail != nullptr)
	{		
		//����̹��� �տ� ������
		SetActorRotation(player->GetActorRotation());
		SetActorLocation(player->rightAim->GetComponentLocation());
	}
}

void AScrew::AttachNailtoScrew(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	attachedNail = Cast<ANail>(OtherActor);
	if(attachedNail != nullptr)
	{
		isAttaching = true;
		attachedNail->boxComp->SetSimulatePhysics(false);		
		attachedNail->boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//attachedNail->AttachToComponent(boxComp, FAttachmentTransformRules::KeepWorldTransform);
		attachedNail->SetActorLocation(attachedNail->GetActorLocation());
		//auto y = attachedNail->GetActorRotation().Yaw;
		//auto p = attachedNail->GetActorRotation().Pitch;
		//DisableInput(attachedNail->GetActorRotation().Yaw);

		AttachNailProcess();
	}
}

void AScrew::AttachNailProcess()
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

void AScrew::ComingoutNails()
{
	//������ ������ ��
	attachedNail->SetActorLocation(GetActorLocation() + FVector(50, 0, 0));
}


void AScrew::initNail()
{
	isAttaching = false;
	attachedNail = nullptr;
	initRot = 0;
}

//����̹��� ���翡 ������ ����̹��� ȸ�� ���� ���缭 ���縦 ȸ����Ų��





