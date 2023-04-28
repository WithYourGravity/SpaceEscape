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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	EEnemyState animState;

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
};
