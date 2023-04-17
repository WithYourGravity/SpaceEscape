// Fill out your copyright notice in the Description page of Project Settings.


#include "Nail.h"
#include "Components/BoxComponent.h"
#include "Screw.h"
#include "Kismet/KismetMathLibrary.h"
#include "EscapePlayer.h"

ANail::ANail()
{ 	
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("NAIL Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(20, 20, 50));
	boxComp->SetRelativeRotation(FRotator(0, 0, -90));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NailMesh"));
	meshComp->SetupAttachment(RootComponent);


}

void ANail::BeginPlay()
{
	Super::BeginPlay();
	
	//����̹��� ���翡 ��´�
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ANail::AttachScrewToNail);

	//����̹��� ���翡�� ��������
	boxComp->OnComponentEndOverlap.AddDynamic(this, &ANail::DettachScrewToNail);

	initScrew();	
}

void ANail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANail::AttachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//����̹��� ����� �ε�����
	attachedScrew = Cast<AScrew>(OtherActor);
	if (attachedScrew != nullptr)
	{
		isAttaching = true;
		
		//���簡 ����̹��� ȸ����ų �� �ֵ���
		attachedScrew->boxComp->SetEnableGravity(false);
		attachedScrew->meshComp->SetEnableGravity(false);
		attachedScrew->boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//AttachToActor(screw, FAttachmentTransformRules::SnapToTargetNotIncludingScale);		
		AttachNailProcess();
	}
}

void ANail::AttachNailProcess()
{
	if(isAttaching == true)
	{
		
	}
}

void ANail::DettachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//������ ����̹��� ����
	attachedScrew->boxComp->SetEnableGravity(true);
	attachedScrew->meshComp->SetEnableGravity(true);
	attachedScrew = nullptr;

}

void ANail::CameOutNail()
{
	//�ϴ� ���α�

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

void ANail::initScrew()
{
	isAttaching = false;
	attachedScrew = nullptr;
	//initNailRot = VectorZero();
	initRot = 0;
}

