// Fill out your copyright notice in the Description page of Project Settings.


#include "Screw.h"
#include "Components/BoxComponent.h"
#include "EscapePlayer.h"

AScrew::AScrew()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("NAIL Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(20, 20, 50));
	boxComp->SetRelativeRotation(FRotator(0, 0, -90));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NailMesh"));
	meshComp->SetupAttachment(RootComponent);
}

void AScrew::BeginPlay()
{
	Super::BeginPlay();

	//����̹��� ���翡 ��´�
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &AScrew::AttachScrewToNail);

	//����̹��� ���翡�� ��������
	//boxComp->OnComponentEndOverlap.AddDynamic(this, &AScrew::DettachScrewToNail);

	initScrewDriver();
}

void AScrew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
void AScrew::AttachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//����̹��� ����� �ε�����
	attachedScrew = Cast<AScrew>(OtherActor);
	if (attachedScrew != nullptr)
	{
		isAttaching = true;
		boxComp->SetSimulatePhysics(false);
		//attachedScrew->boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//AttachToComponent(attachedScrew->boxComp, FAttachmentTransformRules::KeepWorldTransform);
		//AttachNailProcess();
	}
}

void AScrew::AttachNailProcess()
{
	if(isAttaching == true)
	{

	}
}

void AScrew::DettachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//������ ����̹��� ����
	//attachedScrew->boxComp->SetEnableGravity(true);
	//attachedScrew->meshComp->SetEnableGravity(true);
	//attachedScrew = nullptr;

}

void AScrew::CameOutNail()
{
	//�ϴ� ���� ����

	//���簡 ȸ���ϴ� ���⿡ ���缭 ����̹��� ȸ��
	attachedScrew->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());

	//���� ȸ����Ű�� ���� ����Ͽ� ���� ������ ���ð��̴�
	//���� ȸ����Ű�� �� = ����̹� ȸ���� ��
	//���� ������ ������ ��

	//�� ������ �ٴ����� ��������
	SetActorLocation(GetActorLocation() + FVector(comeOutNailDist, 0, 0));
	rotCount++;
	if (rotCount == 5)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		boxComp->SetEnableGravity(true);
		meshComp->SetEnableGravity(true);
	}
}
*/
void AScrew::initScrewDriver()
{
	isAttaching = false;
	attachedScrew = nullptr;
	//initNailRot = VectorZero();
	initRot = 0;
}




