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

	//드라이버가 나사에 닿는다
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ANail::AttachScrewToNail);

	//드라이버가 나사에서 떨어진다
	//boxComp->OnComponentEndOverlap.AddDynamic(this, &AScrew::DettachScrewToNail);

	initScrew();
}

void ANail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(isAttacing)
	{
		// 회전을 하는가 ?

		// 하고 있다면, 이것은 시계방향인가 역방향인가 ? 
		//회전 하면 양의 방향으로 회전인지, 음의 방향으로 회전인지 확인한다

		double currentPitch = attachedScrew->GetActorRotation().Pitch;

		// if 회전한 각도가 x도를 넘었으면{  actor.move   } 
		if(abs(initPitch - currentPitch) > 1)
		{
			if (initPitch - currentPitch < 0)
			{
				//오른쪽
			}
			else
			{
				//왼쪽
			}
		}
		
	}
}

void ANail::AttachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//드라이버가 나사에 붙으면
	attachedScrew = Cast<AScrew>(OtherActor);
	if (attachedScrew != nullptr)
	{
		
		//만난 순간에 초기 회전값을 하나 저장 
		initPitch = attachedScrew->GetActorRotation().Pitch;		
		
		//회전 방향에 따라 x의 양의 방향으로 나올지, 음의 방향으로 나올지 설정할것
		//1. 회전하는가 확인한다
		//2. 회전안하면 else
		//3. 회전 하면 양의 방향으로 회전인지, 음의 방향으로 회전인지 확인한다
		//4. 양의 방향으로 회전하면 x 방향으로 나온다
		//5. 음의 방향으로 회전하면 x *-1방향으로 나온다.	
				
		isAttacing = true;



		//나사가 드라이버를 회전시킬것이다. 드라이버가 닿으면
		attachedScrew->boxComp->SetEnableGravity(false);
		attachedScrew->meshComp->SetEnableGravity(false);
		//AttachToActor(screw, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		//나사가 회전하는 방향에 맞춰서 드라이버를 회전
		attachedScrew->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());

		//못을 회전시키는 값에 비례하여 못이 앞으로 나올것이다
		//못을 회전시키는 값 = 드라이버 회전한 값
		//FRotator nailRotValue = GetActorRotation();
		//못이 앞으로 나오는 값
		//FVector nailMoveValue = nailLoc + GetActorLocation();

		//다 나오면 바닥으로 떨어지게
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
