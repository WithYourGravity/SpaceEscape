// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "ResearcherEnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class SPACEESCAPE_API UResearcherEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	EEnemyState animState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	EEnemyMoveSubState animMoveState;

	// ���� ���� ������� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	bool bAttackPlay = false;
	
	// ���� �ִϸ��̼� ������ �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable, Category = "FSMEvent")
	void OnEndAttackAnimation();
	
	// �ǰ� �ִϸ��̼� ��� �Լ�
	UFUNCTION(BlueprintImplementableEvent, Category = "FSMEvent")
	void PlayDamageAnim(FName sectionName);

	// ���� ���� �ִϸ��̼� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	bool bDieDone = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float forwardVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float rightVelocity;

	UPROPERTY(BlueprintReadOnly)
	class AResearcherEnemy* owner;
};
