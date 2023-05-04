// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoLaser.h"

#include "NiagaraComponent.h"

APuzzleRoomTwoLaser::APuzzleRoomTwoLaser()
{
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);

	laserStartMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("laserStartMeshComp"));
	laserStartMeshComp->SetupAttachment(RootComponent);

	laserNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("laserNiagaraComp"));
	laserNiagaraComp->SetupAttachment(RootComponent);

}

void APuzzleRoomTwoLaser::BeginPlay()
{
	Super::BeginPlay();

	
}

void APuzzleRoomTwoLaser::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// �ش� �������� ���Խø� �����ϰ� ó��

	LetsTraceLaser(laserStartMeshComp);
}

void APuzzleRoomTwoLaser::LetsTraceLaser(class UStaticMeshComponent* startPointMeshComp)
{
	FHitResult hitResult;
	FVector startLoc = startPointMeshComp->GetComponentLocation();
	FVector endLoc = startLoc + startPointMeshComp->GetForwardVector() * 1500;
	FCollisionQueryParams params;
	params.AddIgnoredComponent(startPointMeshComp);
	
	GetWorld()->LineTraceSingleByChannel(hitResult, startLoc, endLoc, ECC_Visibility, params);
	//DrawDebugLine(GetWorld(), startLoc, hitResult.ImpactPoint, FColor::Blue, false, -1, 0, 5);

	// Niagara User Parameter�� ������ �����ǥ�� ������ ����
	laserNiagaraComp->SetVectorParameter(FName("LaserEnd"), hitResult.ImpactPoint - startLoc);
}
