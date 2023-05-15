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

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	bool bAttackPlay = false;
	
	// 공격 애니메이션 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable, Category = "FSMEvent")
	void OnEndAttackAnimation();
	
	// 피격 애니메이션 재생 함수
	UFUNCTION(BlueprintImplementableEvent, Category = "FSMEvent")
	void PlayDamageAnim(FName sectionName);

	// 죽음 상태 애니메이션 종료 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	bool bDieDone = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float forwardVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float rightVelocity;

	UPROPERTY(BlueprintReadOnly)
	class AResearcherEnemy* owner;
};
