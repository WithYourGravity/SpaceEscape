// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "AIController.h"
#include "DamageWidget.h"
#include "Doors.h"
#include "EscapePlayer.h"
#include "NavigationSystem.h"
#include "PlayerInfoWidget.h"
#include "ResearcherEnemy.h"
#include "ResearcherEnemyAnim.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	door = Cast<ADoors>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoors::StaticClass()));
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
	case EEnemyState::STUCK:
		TickStuck();
		break;
	case EEnemyState::KNOCK:
		TickKnock();
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
		SetState(EEnemyState::DIE);
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// ���� �ִϸ��̼� ���
		FString sectionName = FString(TEXT("Die"));
		if (moveState == EEnemyMoveSubState::CRAWL)
		{
			sectionName += FString(TEXT("Crawl"));
			me->SetActorLocation(me->GetActorLocation() + FVector(0, 0, 73));
		}
		else
		{
			int32 index = FMath::RandRange(0, 1);
			sectionName += FString::Printf(TEXT("%d"), index);
		}
		
		anim->PlayDamageAnim(*sectionName);

		bIsDying = true;
	}
	else
	{
		SetState(EEnemyState::DAMAGE);

		// �ǰ� �ִϸ��̼� ���
		FString sectionName = FString(TEXT("Damage"));
		if (moveState == EEnemyMoveSubState::CRAWL)
		{
			sectionName += FString(TEXT("Crawl"));
		}
		else if (damagePart == EEnemyHitPart::CHEST)
		{
			//int32 index = FMath::RandRange(0, 1);
			sectionName += FString::Printf(TEXT("Chest%d"), 1);
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
			if (moveState == EEnemyMoveSubState::INJUREDWALKLEFT)
			{
				moveState = EEnemyMoveSubState::CRAWL;
			}
			else
			{
				moveState = EEnemyMoveSubState::INJUREDWALKRIGHT;
			}
		}
		else if (damagePart == EEnemyHitPart::RIGHTARM)
		{
			sectionName += FString(TEXT("RightArm"));
		}
		else if (damagePart == EEnemyHitPart::RIGHTLEG)
		{
			sectionName += FString(TEXT("RightLeg"));
			if (moveState == EEnemyMoveSubState::INJUREDWALKRIGHT)
			{
				moveState = EEnemyMoveSubState::CRAWL;
			}
			else
			{
				moveState = EEnemyMoveSubState::INJUREDWALKLEFT;
			}
		}
		anim->PlayDamageAnim(*sectionName);
	}
	
	anim->animMoveState = moveState;
	if (moveState == EEnemyMoveSubState::INJUREDWALKLEFT || moveState == EEnemyMoveSubState::INJUREDWALKRIGHT
	)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	}
	else if (moveState == EEnemyMoveSubState::CRAWL)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 100.0f;
		me->GetCapsuleComponent()->SetCapsuleRadius(15.0f);
		me->GetCapsuleComponent()->SetCapsuleHalfHeight(15.0f);
		me->GetMesh()->SetRelativeLocation(FVector(0, 0, -15));
		me->SetActorLocation(me->GetActorLocation() + FVector(0, 0, -73));
	}

	ai->StopMovement();
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	if (result)
	{
		dest = loc.Location;
	}
	return result;
}

void UEnemyFSM::AttackPlayer()
{
	if (target && target->GetHP() <= 0)
	{
		return;
	}

	FCollisionQueryParams params;
	params.AddIgnoredActor(me);
	FVector startPos = me->GetActorLocation() + me->GetActorForwardVector() * 60.0f;
	FVector endPos = startPos + me->GetActorForwardVector() * 30.0f;

	FHitResult HitInfo;
	bool bHit = GetWorld()->SweepSingleByObjectType(HitInfo, startPos, endPos, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeCapsule(30.0f, 78.0f), params);

	//DrawDebugCapsule(GetWorld(), startPos, 78.0f, 30.0f, FQuat::Identity, FColor::Red, false, 2.0f, 0, 1);
	//DrawDebugCapsule(GetWorld(), endPos, 78.0f, 30.0f, FQuat::Identity, FColor::Red, false, 2.0f, 0, 1);

	if (!bHit)
	{
		return;
	}

	ShowAttackPlayerEffect();
	target->SubtractHP(power);
	target->infoUI->PrintCurrentHPPercent();

	if (target->GetHP() == 0)
	{
		target->Die();
	}
}

void UEnemyFSM::ShowAttackPlayerEffect()
{
	if (target)
	{
		target->damageWidgetComp->SetVisibility(true);
		target->damageUI->PlayDamageAnimation();
	}
}

void UEnemyFSM::HiddenAttackPlayerEffect()
{
	if (target)
	{
		target->damageWidgetComp->SetVisibility(false);
	}
}

void UEnemyFSM::TickIdle()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime)
	{
		if (target != nullptr)
		{
			SetState(EEnemyState::MOVE);

			// �ִϸ��̼� ���� ����ȭ
			anim->animMoveState = moveState;

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

	
	// ���� ������ �� ���ڸ��� ���� �ִ� �����̸� enemy�鳢�� JAMMED
	if (door && !door->bIsOpenOverlaping)
	{
		currentTime += GetWorld()->DeltaTimeSeconds;
		if (currentTime <= 0.1f)
		{
			prevLocation = me->GetActorLocation();
		}
		else if (currentTime >= 3.0f)
		{
			curLocation = me->GetActorLocation();
			if (FVector::Distance(prevLocation, curLocation) <= 10.0f)
			{
				SetState(EEnemyState::STUCK);
			}

			currentTime = 0.0f;
		}
	}

	// ���� �����ְ� �� �����̿� ������ KNOCK
	if (door && door->bIsOpenOverlaping && bIsOverlapDoor)
	{
		SetState(EEnemyState::KNOCK);
	}

	// target �� ��������� ���� ���·� ��ȯ
	if (dir.Size() < attackRange)
	{
		// �� ã�� ��� ����
		ai->StopMovement();
		
		SetState(EEnemyState::ATTACK);
		
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
		currentTime = 0.0f;
		anim->bAttackPlay = true;
	}

	// Ÿ���� ���� ������ ����� ���¸� �̵����� ��ȯ
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		SetState(EEnemyState::MOVE);

		GetRandomPositionInNavMesh(me->GetActorLocation(), randomPositionRadius, randomPos);
	}
}

void UEnemyFSM::TickDamage()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		SetState(EEnemyState::MOVE);
	}
}

void UEnemyFSM::TickDie()
{
	if (anim->bDieDone == false)
	{
		return;
	}

	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > 1)
	{
		me->Destroy();
	}
}

void UEnemyFSM::TickStuck()
{
	// ���ο� ���� ��ġ ��������
	GetRandomPositionInNavMesh(me->GetActorLocation(), 200.0f, randomPos);

	// ���� ��ġ�� �̵�
	auto result = ai->MoveToLocation(randomPos);
	// �������� �����ϸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal || result == EPathFollowingRequestResult::Failed)
	{
		SetState(EEnemyState::MOVE);

		// ���ο� ���� ��ġ ��������
		GetRandomPositionInNavMesh(me->GetActorLocation(), randomPositionRadius, randomPos);
	}
}

void UEnemyFSM::TickKnock()
{
	if (door && !door->bIsOpenOverlaping)
	{
		SetState(EEnemyState::MOVE);
	}
}

void UEnemyFSM::SetState(EEnemyState next)
{
	state = next;
	anim->animState = next;
	currentTime = 0.0f;
}

