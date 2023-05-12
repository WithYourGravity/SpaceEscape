// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoLaser.h"

#include "NiagaraComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "EscapePlayer.h"
#include "RoomManager.h"
#include "Kismet/GameplayStatics.h"

APuzzleRoomTwoLaser::APuzzleRoomTwoLaser()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);

	laserBaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("laserBaseMeshComp"));
	laserBaseMeshComp->SetupAttachment(RootComponent);
	laserBaseMeshComp->SetRelativeScale3D(FVector(0.1f));
	laserBaseMeshComp->SetRelativeRotation(FRotator(0, 90.f, 0));
	laserBaseMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));

	laserTopMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("laserTopMeshComp"));
	laserTopMeshComp->SetupAttachment(RootComponent);
	laserTopMeshComp->SetRelativeLocation(FVector(0, 0, 20.f));
	laserTopMeshComp->SetRelativeRotation(FRotator(0, 90.f, 0));
	laserTopMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	
	laserNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("laserNiagaraComp"));
	laserNiagaraComp->SetupAttachment(laserTopMeshComp, FName("LaserStart"));
	laserNiagaraComp->SetRelativeRotation(FRotator(90.f, 0, 0));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempLaserBase(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_Laser_Base.SM_Laser_Base'"));
	if (tempLaserBase.Succeeded())
	{
		laserBaseMeshComp->SetStaticMesh(tempLaserBase.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempLaserTop(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_LaserTop.SM_LaserTop'"));
	if (tempLaserTop.Succeeded())
	{
		laserTopMeshComp->SetStaticMesh(tempLaserTop.Object);
	}

	test = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("test"));
	test->SetupAttachment(RootComponent);
}

void APuzzleRoomTwoLaser::BeginPlay()
{
	Super::BeginPlay();

	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoomManager::StaticClass()));
}

void APuzzleRoomTwoLaser::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// �ش� �������� ���Խø� �����ϰ� ó��
	if (rm->GetCurrentPlayingStage() == 2)
	{
		LetsTraceLaser();
		ChangeLaserColor();
	}

	// �׽�Ʈ
	curTime += DeltaSeconds * 0.1f;
	test->SetRelativeScale3D(FVector(FMath::Clamp(FMath::Lerp(1.f, 0, curTime), 0, 1)));
	UE_LOG(LogTemp, Warning, TEXT("%f"), curTime);
}

void APuzzleRoomTwoLaser::LetsTraceLaser()
{
	// ����Ʈ���̽� ����
	FHitResult hitResult;
	FVector startLoc = laserTopMeshComp->GetSocketLocation(FName("LaserStart"));
	FVector endLoc = startLoc + (laserTopMeshComp->GetSocketLocation(FName("LaserStart")) - laserTopMeshComp->GetSocketLocation(FName("LaserBack"))).GetSafeNormal() * 2000;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(hitResult, startLoc, endLoc, ECC_Visibility, params);

	//DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Blue, false, -1, 0, 4);

	// �������� �浹������ �հ� �������� �ʰ� NiagaraComp�� ��ġ ����
	// hitResult�� world��ǥ�̰� vectorParameter�� relative��ǥ�� ���ϱ� ������ ��ȯ�ؼ� ����
	if (hitResult.bBlockingHit)
	{
		FTransform trans;
		trans.SetLocation(hitResult.ImpactPoint);
		FTransform nt = trans.GetRelativeTransform(laserNiagaraComp->GetComponentTransform());
		laserNiagaraComp->SetVectorParameter(FName("LaserEnd"), nt.GetLocation());
	}

	// ���� �÷��̾ ��Ҵٸ�
	auto pl = Cast<AEscapePlayer>(hitResult.GetActor());
	if (pl)
	{
		LaserPlayerTouched();
	}
}

void APuzzleRoomTwoLaser::LaserPlayerTouched()
{
	if (!bTouchLaserOnce)
	{
		UE_LOG(LogTemp, Warning, TEXT("Laser Player Touched!"));
		bTouchLaserOnce = true;
	}
}

void APuzzleRoomTwoLaser::ChangeLaserColor()
{
	float currentLaserRoll = laserTopMeshComp->GetRelativeRotation().Roll;
	if (!bChangeColorOnce && (currentLaserRoll > 90.f || currentLaserRoll < -90.f))
	{
		laserNiagaraComp->SetColorParameter(FName("Color"), FLinearColor::Green);
		bChangeColorOnce = true;
	}
}
