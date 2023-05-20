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

	// ���� ����
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

	// �� ã�� ����� ���� ��ġ
	FVector randomPos;

	// ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float idleDelayTime = 2.0f;

	// ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	float currentTime = 0.0f;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackRange = 100.0f;

	// ���� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackDelayTime = 3.0f;

	// ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	int32 maxHP = 9;

	// �ǰ� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float damageDelayTime = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = "FSM")
	float randomPositionRadius = 500.0f;

	bool bIsDying = false;

	// �ǰ� �˸� �̺�Ʈ �Լ�
	void OnDamageProcess(int32 damageValue, EEnemyHitPart damagePart);

	// ���� ��ġ ��������
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	UFUNCTION(BlueprintCallable)
	void AttackPlayer();
	
private:
	// ��� ����
	void TickIdle();
	// �̵� ����
	void TickMove();
	// ���� ����
	void TickAttack();
	// �ǰ� ����
	void TickDamage();
	// ���� ����
	void TickDie();

	void SetState(EEnemyState next);

	int32 HP;

	int32 power = 10;
};
