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

	// 해당 스테이지 진입시만 동작하게 처리
	if (rm->GetCurrentPlayingStage() == 2)
	{
		LetsTraceLaser();
		ChangeLaserColor();
	}

	// 테스트
	curTime += DeltaSeconds * 0.1f;
	test->SetRelativeScale3D(FVector(FMath::Clamp(FMath::Lerp(1.f, 0, curTime), 0, 1)));
	UE_LOG(LogTemp, Warning, TEXT("%f"), curTime);
}

void APuzzleRoomTwoLaser::LetsTraceLaser()
{
	// 라인트레이스 관련
	FHitResult hitResult;
	FVector startLoc = laserTopMeshComp->GetSocketLocation(FName("LaserStart"));
	FVector endLoc = startLoc + (laserTopMeshComp->GetSocketLocation(FName("LaserStart")) - laserTopMeshComp->GetSocketLocation(FName("LaserBack"))).GetSafeNormal() * 2000;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(hitResult, startLoc, endLoc, ECC_Visibility, params);

	//DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Blue, false, -1, 0, 4);

	// 레이저가 충돌지점을 뚫고 지나가지 않게 NiagaraComp에 위치 전달
	// hitResult는 world좌표이고 vectorParameter는 relative좌표를 원하기 때문에 변환해서 전달
	if (hitResult.bBlockingHit)
	{
		FTransform trans;
		trans.SetLocation(hitResult.ImpactPoint);
		FTransform nt = trans.GetRelativeTransform(laserNiagaraComp->GetComponentTransform());
		laserNiagaraComp->SetVectorParameter(FName("LaserEnd"), nt.GetLocation());
	}

	// 만약 플레이어가 닿았다면
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
