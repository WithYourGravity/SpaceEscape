// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "EscapePlayer.h"
#include "ResearcherEnemy.h"
#include "ResearcherEnemyAnim.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	target = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	me = Cast<AResearcherEnemy>(GetOwner());

	anim = Cast<UResearcherEnemyAnim>(me->GetMesh()->GetAnimInstance());

	HP = maxHP;
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (state)
	{
	case EEnemyState::IDLE:
		TickIdle();
		break;
	case EEnemyState::MOVE:
		TickMove();
		break;
	case EEnemyState::ATTACK:
		TickAttack();
		break;
	case EEnemyState::DAMAGE:
		TickDamage();
		break;
	case EEnemyState::DIE:
		TickDie();
		break;
	default:
		break;
	}
}

void UEnemyFSM::OnDamageProcess(int32 damageValue)
{
	HP -= damageValue;

	if (HP <= 0)
	{
		state = EEnemyState::DIE;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// ���� �ִϸ��̼� ���
		anim->PlayDamageAnim(TEXT("Die"));
	}
	else
	{
		state = EEnemyState::DAMAGE;

		currentTime = 0;

		// �ǰ� �ִϸ��̼� ���
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(*sectionName);
	}

	anim->animState = state;
}

void UEnemyFSM::TickIdle()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime)
	{
		if (target != nullptr)
		{
			state = EEnemyState::MOVE;
			currentTime = 0.0f;

			// �ִϸ��̼� ���� ����ȭ
			anim->animState = state;
		}
	}
}

void UEnemyFSM::TickMove()
{
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	me->AddMovementInput(dir.GetSafeNormal());

	if (dir.Size() < attackRange)
	{
		state = EEnemyState::ATTACK;

		// �ִϸ��̼� ���� ����ȭ
		anim->animState = state;
		// ���� �ִϸ��̼� ��� Ȱ��ȭ
		anim->bAttackPlay = true;
		// ���� ���� ��ȯ �� ��� �ð��� �ٷ� �������� ó��
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::TickAttack()
{
	// ���� �ð��� �� ���� ����
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATTACK"));

		currentTime = 0.0f;
		anim->bAttackPlay = true;
	}

	// Ÿ���� ���� ������ ����� ���¸� �̵����� ��ȯ
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		state = EEnemyState::MOVE;
		anim->animState = state;
	}
}

void UEnemyFSM::TickDamage()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		state = EEnemyState::IDLE;
		currentTime = 0.0f;
		anim->animState = state;
	}
}

void UEnemyFSM::TickDie()
{
	if (anim->bDieDone == false)
	{
		return;
	}

	currentTime += GetWorld()->DeltaTimeSeconds;

	// P = P0 + vt
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector(0, 0, -1) * 200 * GetWorld()->DeltaTimeSeconds;

	me->SetActorLocation(p0 + vt);

	if (currentTime > 1)
	{
		me->Destroy();
	}
}

