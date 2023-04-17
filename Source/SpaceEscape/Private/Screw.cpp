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
		//드라이버가 손에 붙으면
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
	//손이 회전하는 만큼 드라이버가 움직이는데, 나사도 같이 움직이는것

	//드라이버의 이전 회전
	//auto prevScrewRot = GetActorRotation().Roll - 10;

	//나사의 회전값
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
	//앞으로 나오는 값
	attachedNail->SetActorLocation(GetActorLocation() + FVector(50, 0, 0));
}


void AScrew::initNail()
{
	isAttaching = false;
	attachedNail = nullptr;
	initRot = 0;
}

//드라이버가 나사에 닿으면 드라이버의 회전 값에 맞춰서 나사를 회전시킨다





