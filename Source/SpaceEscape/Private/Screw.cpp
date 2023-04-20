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

	//드라이버가 나사에 닿는다
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &AScrew::AttachScrewToNail);

	//드라이버가 나사에서 떨어진다
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
	//드라이버가 나사와 부딪히면
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
	//못에서 드라이버를 빼면
	//attachedScrew->boxComp->SetEnableGravity(true);
	//attachedScrew->meshComp->SetEnableGravity(true);
	//attachedScrew = nullptr;

}

void AScrew::CameOutNail()
{
	//일단 빼둔 내용

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
*/
void AScrew::initScrewDriver()
{
	isAttaching = false;
	attachedScrew = nullptr;
	//initNailRot = VectorZero();
	initRot = 0;
}




