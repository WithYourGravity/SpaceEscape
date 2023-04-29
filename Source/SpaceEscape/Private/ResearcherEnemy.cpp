// Fill out your copyright notice in the Description page of Project Settings.


#include "ResearcherEnemy.h"
#include "EnemyFSM.h"
#include "NavigationInvokerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AResearcherEnemy::AResearcherEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/YSY/Assets/Enemy/Zombie/Agony.Agony'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;

	enemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("enemyFSM"));

	navInvokerComp = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("navInvokerComp"));
	navInvokerComp->SetGenerationRadii(500.0f, 800.0f);

	// �ִϸ��̼� �������Ʈ �Ҵ�
	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/YSY/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
	if (tempAnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnimClass.Class);
	}

	// ���忡 ��ġ�ǰų� ������ �� �ڵ����� AIController���� Possess�� �� �ֵ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AResearcherEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResearcherEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AResearcherEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

