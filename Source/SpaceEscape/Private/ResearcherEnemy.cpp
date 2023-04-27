// Fill out your copyright notice in the Description page of Project Settings.


#include "ResearcherEnemy.h"
#include "EnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AResearcherEnemy::AResearcherEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/YSY/Assets/Enemy/Vanguard_By_T__Choonyung.Vanguard_By_T__Choonyung'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;

	enemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("enemyFSM"));

	// 애니메이션 블루프린트 할당
	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/YSY/Blueprints/ABP_ResearcherEnemy.ABP_ResearcherEnemy_C'"));
	if (tempAnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnimClass.Class);
	}
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

