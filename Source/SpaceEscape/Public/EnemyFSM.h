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

	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	EEnemyState state = EEnemyState::IDLE;

	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class AEscapePlayer* target;

	UPROPERTY()
	class AResearcherEnemy* me;

	// ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float idleDelayTime = 2.0f;

	// ��� �ð�
	float currentTime = 0.0f;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackRange = 200.0f;

	// ���� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackDelayTime = 2.0f;

	// ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	int32 maxHP = 10;

	// �ǰ� ��� �ð�
	UPROPERTY(EditAnywhere, Category = "FSM")
	float damageDelayTime = 2.0f;

	// �ǰ� �˸� �̺�Ʈ �Լ�
	void OnDamageProcess(int32 damageValue);

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

	int32 HP;
	
};
