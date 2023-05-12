// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "EnemyFSM.h"
#include "ResearcherEnemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(3.0f);
	sphereComp->SetCollisionProfileName(FName("BulletPreset"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Pistol/Bullet.Bullet'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		meshComp->SetRelativeScale3D(FVector(0.5f));
	}

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
	movementComp->SetUpdatedComponent(sphereComp);
	movementComp->InitialSpeed = 1000;
	movementComp->MaxSpeed = 1000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.5f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
	sphereComp->SetGenerateOverlapEvents(true);

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
