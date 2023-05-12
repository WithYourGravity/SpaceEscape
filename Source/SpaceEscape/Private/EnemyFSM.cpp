// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "AIController.h"
#include "EscapePlayer.h"
#include "NavigationSystem.h"
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

	ai = Cast<AAIController>(me->GetController());

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

void UEnemyFSM::OnDamageProcess(int32 damageValue, EEnemyHitPart damagePart)
{
	HP -= damageValue;

	if (HP <= 0)
	{
		state = EEnemyState::DIE;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// ���� �ִϸ��̼� ���
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Die%d"), index);
		anim->PlayDamageAnim(*sectionName);

		bIsDying = true;
	}
	else
	{
		state = EEnemyState::DAMAGE;

		currentTime = 0;

		// �ǰ� �ִϸ��̼� ���
		FString sectionName = FString(TEXT("Damage"));
		if (damagePart == EEnemyHitPart::CHEST)
		{
			int32 index = FMath::RandRange(0, 1);
			sectionName += FString::Printf(TEXT("Chest%d"), index);
		}
		else if (damagePart == EEnemyHitPart::HEAD)
		{
			sectionName += FString(TEXT("Head"));
		}
		else if (damagePart == EEnemyHitPart::LEFTARM)
		{
			sectionName += FString(TEXT("LeftArm"));
		}
		else if (damagePart == EEnemyHitPart::LEFTLEG)
		{
			sectionName += FString(TEXT("LeftLeg"));
		}
		else if (damagePart == EEnemyHitPart::RIGHTARM)
		{
			sectionName += FString(TEXT("RightArm"));
		}
		else if (damagePart == EEnemyHitPart::RIGHTLEG)
		{
			sectionName += FString(TEXT("RightLeg"));
		}
		anim->PlayDamageAnim(*sectionName);
	}

	anim->animState = state;

	ai->StopMovement();
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
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

			// ���� ������ ��ġ �����ֱ�
			GetRandomPositionInNavMesh(me->GetActorLocation(), randomPositionRadius, randomPos);
		}
	}
}

void UEnemyFSM::TickMove()
{
	FVector destination = target->GetActorLocation();

	FVector dir = destination - me->GetActorLocation();

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	// ������ �� ã�� ��� ������ �˻�
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	// �� ã�⸦ ���� ���� ����
	ai->BuildPathfindingQuery(req, query);
	// �� ã�� ��� ��������
	FPathFindingResult r = ns->FindPathSync(query);

	// ������������ ��ã�� ���� ���� Ȯ��
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		// ���� ��ġ�� �̵�
		auto result = ai->MoveToLocation(randomPos);
		// �������� �����ϸ�
		if (result == EPathFollowingRequestResult::AlreadyAtGoal || result == EPathFollowingRequestResult::Failed)
		{
			// ���ο� ���� ��ġ ��������
			GetRandomPositionInNavMesh(me->GetActorLocation(), randomPositionRadius, randomPos);
		}
	}

	// target �� ��������� ���� ���·� ��ȯ
	if (dir.Size() < attackRange)
	{
		// �� ã�� ��� ����
		ai->StopMovement();

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

		GetRandomPositionInNavMesh(me->GetActorLocation(), randomPositionRadius, randomPos);
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
	//FVector p0 = me->GetActorLocation();
	//FVector vt = FVector(0, 0, -1) * 200 * GetWorld()->DeltaTimeSeconds;

	//me->SetActorLocation(p0 + vt);

	if (currentTime > 1)
	{
		me->Destroy();
	}
}

