// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,
	MOVE,
	ATTACK,
	DAMAGE,
	DIE,
	KNOCK,
	JAMMED,
};

UENUM(BlueprintType)
enum class EEnemyHitPart : uint8
{
	CHEST,
	HEAD,
	LEFTARM,
	LEFTLEG,
	RIGHTARM,
	RIGHTLEG,
	NONE,
};

UENUM(BlueprintType)
enum class EEnemyMoveSubState : uint8
{
	NORMAL,
	INJUREDWALKLEFT,
	INJUREDWALKRIGHT,
	CRAWL,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEESCAPE_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	EEnemyState state = EEnemyState::IDLE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	EEnemyMoveSubState moveState = EEnemyMoveSubState::NORMAL;

	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class AEscapePlayer* target;

	UPROPERTY()
	class AResearcherEnemy* me;

	UPROPERTY()
	class UResearcherEnemyAnim* anim;

	UPROPERTY()
	class AAIController* ai;

	// 길 찾기 수행시 랜덤 위치
	FVector randomPos;

	// 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float idleDelayTime = 2.0f;

	// 경과 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	float currentTime = 0.0f;

	// 공격 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackRange = 100.0f;

	// 공격 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackDelayTime = 3.0f;

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	int32 maxHP = 9;

	// 피격 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float damageDelayTime = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = "FSM")
	float randomPositionRadius = 500.0f;

	bool bIsDying = false;

	// 피격 알림 이벤트 함수
	void OnDamageProcess(int32 damageValue, EEnemyHitPart damagePart);

	// 랜덤 위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	UFUNCTION(BlueprintCallable)
	void AttackPlayer();
	
private:
	// 대기 상태
	void TickIdle();
	// 이동 상태
	void TickMove();
	// 공격 상태
	void TickAttack();
	// 피격 상태
	void TickDamage();
	// 죽음 상태
	void TickDie();

	void SetState(EEnemyState next);

	int32 HP;

	int32 power = 10;
};
