// Fill out your copyright notice in the Description page of Project Settings.


#include "ResearcherEnemyAnim.h"
#include "ResearcherEnemy.h"

void UResearcherEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	owner = Cast<AResearcherEnemy>(TryGetPawnOwner());
}

void UResearcherEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (owner == nullptr)
	{
		return;
	}

	FVector velocity = owner->GetVelocity();
	rightVelocity = FVector::DotProduct(velocity, owner->GetActorRightVector());
	forwardVelocity = FVector::DotProduct(velocity, owner->GetActorForwardVector());
}

void UResearcherEnemyAnim::OnEndAttackAnimation()
{
	bAttackPlay = false;
}
