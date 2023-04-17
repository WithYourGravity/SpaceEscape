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
	
	//드라이버가 나사에 닿는다
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ANail::AttachScrewToNail);

	//드라이버가 나사에서 떨어진다
	boxComp->OnComponentEndOverlap.AddDynamic(this, &ANail::DettachScrewToNail);

	initScrew();	
}

void ANail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANail::AttachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//드라이버가 나사와 부딪히면
	attachedScrew = Cast<AScrew>(OtherActor);
	if (attachedScrew != nullptr)
	{
		isAttaching = true;
		
		//나사가 드라이버를 회전시킬 수 있도록
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
	//못에서 드라이버를 빼면
	attachedScrew->boxComp->SetEnableGravity(true);
	attachedScrew->meshComp->SetEnableGravity(true);
	attachedScrew = nullptr;

}

void ANail::CameOutNail()
{
	//일단 빼두기

	//나사가 회전하는 방향에 맞춰서 드라이버를 회전
	attachedScrew->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());

	//못을 회전시키는 값에 비례하여 못이 앞으로 나올것이다
	//못을 회전시키는 값 = 드라이버 회전한 값		
	//못이 앞으로 나오는 값

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

void ANail::initScrew()
{
	isAttaching = false;
	attachedScrew = nullptr;
	//initNailRot = VectorZero();
	initRot = 0;
}

