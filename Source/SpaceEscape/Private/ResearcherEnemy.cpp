// Fill out your copyright notice in the Description page of Project Settings.


#include "ResearcherEnemy.h"
#include "EnemyFSM.h"
#include "NavigationInvokerComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AResearcherEnemy::AResearcherEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.9f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/YSY/Assets/Enemy/Zombie/Agony.Agony'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	enemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("enemyFSM"));

	navInvokerComp = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("navInvokerComp"));
	navInvokerComp->SetGenerationRadii(2000.0f, 2300.0f);

	// 애니메이션 블루프린트 할당
	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/YSY/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
	if (tempAnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnimClass.Class);
	}

	// 월드에 배치되거나 스폰될 때 자동으로 AIController부터 Possess될 수 있도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Sound
	ConstructorHelpers::FObjectFinder<USoundCue> tempAttackSoundCue(TEXT("/Script/Engine.SoundCue'/Game/YSY/Assets/Sound/ZombieAttackCue.ZombieAttackCue'"));
	if (tempAttackSoundCue.Succeeded())
	{
		attackSoundCue = tempAttackSoundCue.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> tempDamageLegSoundCue(TEXT("/Script/Engine.SoundCue'/Game/YSY/Assets/Sound/ZombieAttackCue.ZombieAttackCue'"));
	if (tempDamageLegSoundCue.Succeeded())
	{
		damageLegSoundCue = tempDamageLegSoundCue.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> tempDamageShoulderSoundCue(TEXT("/Script/Engine.SoundCue'/Game/YSY/Assets/Sound/ZombieAttackCue.ZombieAttackCue'"));
	if (tempDamageShoulderSoundCue.Succeeded())
	{
		damageShoulderSoundCue = tempDamageShoulderSoundCue.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> tempDamageSpineSoundCue(TEXT("/Script/Engine.SoundCue'/Game/YSY/Assets/Sound/ZombieAttackCue.ZombieAttackCue'"));
	if (tempDamageSpineSoundCue.Succeeded())
	{
		damageSpineSoundCue = tempDamageSpineSoundCue.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> tempMoveSoundCue(TEXT("/Script/Engine.SoundCue'/Game/YSY/Assets/Sound/ZombieMoveCue.ZombieMoveCue'"));
	if (tempMoveSoundCue.Succeeded())
	{
		moveSoundCue = tempMoveSoundCue.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> tempDieSoundCue(TEXT("/Script/Engine.SoundCue'/Game/YSY/Assets/Sound/ZombieDieCue.ZombieDieCue'"));
	if (tempDieSoundCue.Succeeded())
	{
		dieSoundCue = tempDieSoundCue.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> tempKnockSoundCue(TEXT("/Script/Engine.SoundCue'/Game/YSY/Assets/Sound/ZombieKnockCue.ZombieKnockCue'"));
	if (tempKnockSoundCue.Succeeded())
	{
		knockSoundCue = tempKnockSoundCue.Object;
	}

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("audioComp"));
	audioComp->bAutoActivate = false;
	audioComp->SetupAttachment(RootComponent);
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

