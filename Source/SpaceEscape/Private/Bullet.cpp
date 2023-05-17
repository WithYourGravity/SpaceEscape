// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "EnemyFSM.h"
#include "NiagaraComponent.h"
#include "ResearcherEnemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);
	//meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionProfileName(FName("BulletPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Pistol/Bullet.Bullet'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		meshComp->SetRelativeScale3D(FVector(0.5f));
	}

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
	movementComp->SetUpdatedComponent(meshComp);
	movementComp->InitialSpeed = 2000;
	movementComp->MaxSpeed = 2000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.1f;

	bulletTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("bulletTrailComp"));
	bulletTrailComp->SetupAttachment(RootComponent);
	bulletTrailComp->SetRelativeLocation(FVector(-4.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	meshComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
	meshComp->SetGenerateOverlapEvents(true);

	movementComp->OnProjectileStop.AddDynamic(this, &ABullet::OnStop);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorld()->SpawnActor<AActor>(hitEffect, SweepResult.Location, FRotator(0, 0, 0));

	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(movementComp->Velocity * 1.0f);
		Destroy();
	}

	auto enemy = Cast<AResearcherEnemy>(OtherActor);
	if (enemy)
	{
		if (SweepResult.PhysMaterial.Get())
		{
			FString hitPartName = SweepResult.PhysMaterial.Get()->GetName();

			auto enemyFSM = Cast<UEnemyFSM>(enemy->GetDefaultSubobjectByName(TEXT("enemyFSM")));
			if (enemyFSM)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodEffect, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation(), true);

				if (enemyFSM->bIsDying)
				{
					return;
				}

				if (hitPartName.Contains("Chest"))
				{
					enemyFSM->OnDamageProcess(bulletPower, EEnemyHitPart::CHEST);
				}
				else if (hitPartName.Contains("Head"))
				{
					enemyFSM->OnDamageProcess(bulletPower, EEnemyHitPart::HEAD);
				}
				else if (hitPartName.Contains("LeftArm"))
				{
					enemyFSM->OnDamageProcess(bulletPower, EEnemyHitPart::LEFTARM);
				}
				else if (hitPartName.Contains("LeftLeg"))
				{
					enemyFSM->OnDamageProcess(bulletPower, EEnemyHitPart::LEFTLEG);
				}
				else if (hitPartName.Contains("RightArm"))
				{
					enemyFSM->OnDamageProcess(bulletPower, EEnemyHitPart::RIGHTARM);
				}
				else if (hitPartName.Contains("RightLeg"))
				{
					enemyFSM->OnDamageProcess(bulletPower, EEnemyHitPart::RIGHTLEG);
				}

				Destroy();
			}
		}
	}
}

void ABullet::OnStop(const FHitResult& ImpactResult)
{
	Destroy();
}
