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

		// 죽음 애니메이션 재생
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

		// 피격 애니메이션 재생
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

			// 애니메이션 상태 동기화
			anim->animMoveState = moveState;

			// 최초 랜덤한 위치 정해주기
			GetRandomPositionInNavMesh(me->GetActorLocation(), randomPositionRadius, randomPos);
		}
	}
}

void UEnemyFSM::TickMove()
{
	FVector destination = target->GetActorLocation();

	FVector dir = destination - me->GetActorLocation();

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	// 목적지 길 찾기 경로 데이터 검색
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	// 길 찾기를 위한 쿼리 생성
	ai->BuildPathfindingQuery(req, query);
	// 길 찾기 결과 가져오기
	FPathFindingResult r = ns->FindPathSync(query);

	// 목적지까지의 길찾기 성공 여부 확인
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		// 랜덤 위치로 이동
		auto result = ai->MoveToLocation(randomPos);
		// 목적지에 도착하면
		if (result == EPathFollowingRequestResult::AlreadyAtGoal || result == EPathFollowingRequestResult::Failed)
		{
			// 새로운 랜덤 위치 가져오기
			GetRandomPositionInNavMesh(me->GetActorLocation(), randomPositionRadius, randomPos);
		}
	}

	
	// 문이 열렸을 때 제자리에 오래 있는 상태이면 enemy들끼리 JAMMED
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

	// 문이 닫혀있고 문 가까이에 있으면 KNOCK
	if (door && door->bIsOpenOverlaping && bIsOverlapDoor)
	{
		SetState(EEnemyState::KNOCK);
	}

	// target 과 가까워지면 공격 상태로 전환
	if (dir.Size() < attackRange)
	{
		// 길 찾기 기능 정지
		ai->StopMovement();
		
		SetState(EEnemyState::ATTACK);
		
		// 공격 애니메이션 재생 활성화
		anim->bAttackPlay = true;
		// 공격 상태 전환 시 대기 시간이 바로 끝나도록 처리
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::TickAttack()
{
	// 일정 시간에 한 번씩 공격
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0.0f;
		anim->bAttackPlay = true;
	}

	// 타깃이 공격 범위를 벗어나면 상태를 이동으로 전환
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
	// 새로운 랜덤 위치 가져오기
	GetRandomPositionInNavMesh(me->GetActorLocation(), 200.0f, randomPos);

	// 랜덤 위치로 이동
	auto result = ai->MoveToLocation(randomPos);
	// 목적지에 도착하면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal || result == EPathFollowingRequestResult::Failed)
	{
		SetState(EEnemyState::MOVE);

		// 새로운 랜덤 위치 가져오기
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

