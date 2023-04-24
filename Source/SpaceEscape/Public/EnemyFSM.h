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

	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class AEscapePlayer* target;

	UPROPERTY()
	class AResearcherEnemy* me;

	// 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float idleDelayTime = 2.0f;

	// 경과 시간
	float currentTime = 0.0f;

	// 공격 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackRange = 200.0f;

	// 공격 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackDelayTime = 2.0f;

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	int32 maxHP = 10;

	// 피격 대기 시간
	UPROPERTY(EditAnywhere, Category = "FSM")
	float damageDelayTime = 2.0f;

	// 피격 알림 이벤트 함수
	void OnDamageProcess(int32 damageValue);

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

	int32 HP;
	
};
