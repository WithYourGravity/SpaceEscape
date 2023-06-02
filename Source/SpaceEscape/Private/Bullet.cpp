// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "EnemyFSM.h"
#include "ResearcherEnemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"

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
	movementComp->InitialSpeed = 3000;
	movementComp->MaxSpeed = 3000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.1f;
	movementComp->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	meshComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
	meshComp->SetGenerateOverlapEvents(true);

	movementComp->OnProjectileStop.AddDynamic(this, &ABullet::OnStop);

	UGameplayStatics::SpawnEmitterAttached(bulletTrailComp, meshComp, FName("bulletTrailSocket"));

	SetLifeSpan(5.0f);
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
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bloodEffect, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());

				GetWorld()->SpawnActor<AActor>(bloodDecalEffect, SweepResult.ImpactPoint, FRotator(0, 0, 0));

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
					enemyFSM->OnDamageProcess(bulletPower * 2, EEnemyHitPart::HEAD);
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
			}
		}
	}

	Destroy();
}

void ABullet::OnStop(const FHitResult& ImpactResult)
{
	Destroy();
}
