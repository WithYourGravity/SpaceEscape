// Fill out your copyright notice in the Description page of Project Settings.


#include "Nail.h"
#include "Components/BoxComponent.h"
#include "Screw.h"

ANail::ANail()
{ 	
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("NAIL Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(20, 20, 50));
	boxComp->SetRelativeRotation(FRotator(0, 90, 0));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NailMesh"));
	meshComp->SetupAttachment(RootComponent);


}

void ANail::BeginPlay()
{
	Super::BeginPlay();

	//����̹��� ���翡 ��´�
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ANail::AttachScrewToNail);

	//����̹��� ���翡�� ��������
	//boxComp->OnComponentEndOverlap.AddDynamic(this, &AScrew::DettachScrewToNail);

	initScrew();
}

void ANail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(isAttacing)
	{
		// ȸ���� �ϴ°� ?

		// �ϰ� �ִٸ�, �̰��� �ð�����ΰ� �������ΰ� ? 
		//ȸ�� �ϸ� ���� �������� ȸ������, ���� �������� ȸ������ Ȯ���Ѵ�

		double currentPitch = attachedScrew->GetActorRotation().Pitch;

		// if ȸ���� ������ x���� �Ѿ�����{  actor.move   } 
		if(abs(initPitch - currentPitch) > 1)
		{
			if (initPitch - currentPitch < 0)
			{
				//������
			}
			else
			{
				//����
			}
		}
		
	}
}

void ANail::AttachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//����̹��� ���翡 ������
	attachedScrew = Cast<AScrew>(OtherActor);
	if (attachedScrew != nullptr)
	{
		
		//���� ������ �ʱ� ȸ������ �ϳ� ���� 
		initPitch = attachedScrew->GetActorRotation().Pitch;		
		
		//ȸ�� ���⿡ ���� x�� ���� �������� ������, ���� �������� ������ �����Ұ�
		//1. ȸ���ϴ°� Ȯ���Ѵ�
		//2. ȸ�����ϸ� else
		//3. ȸ�� �ϸ� ���� �������� ȸ������, ���� �������� ȸ������ Ȯ���Ѵ�
		//4. ���� �������� ȸ���ϸ� x �������� ���´�
		//5. ���� �������� ȸ���ϸ� x *-1�������� ���´�.	
				
		isAttacing = true;



		//���簡 ����̹��� ȸ����ų���̴�. ����̹��� ������
		attachedScrew->boxComp->SetEnableGravity(false);
		attachedScrew->meshComp->SetEnableGravity(false);
		//AttachToActor(screw, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		//���簡 ȸ���ϴ� ���⿡ ���缭 ����̹��� ȸ��
		attachedScrew->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());

		//���� ȸ����Ű�� ���� ����Ͽ� ���� ������ ���ð��̴�
		//���� ȸ����Ű�� �� = ����̹� ȸ���� ��
		//FRotator nailRotValue = GetActorRotation();
		//���� ������ ������ ��
		//FVector nailMoveValue = nailLoc + GetActorLocation();

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
}

void ANail::initScrew()
{
	 isAttacing = false;
	 attachedScrew = nullptr;
	 //initNailRot = VectorZero();
	 initPitch = 0;
}
