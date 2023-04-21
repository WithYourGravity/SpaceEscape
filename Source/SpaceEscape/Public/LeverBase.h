// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeverBase.generated.h"

UCLASS()
class SPACEESCAPE_API ALeverBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeverBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* leverBodyComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* leverComp;
	UPROPERTY(EditDefaultsOnly)
	class UGrabComponent* grabComp;

	UFUNCTION()
	void ChangeIsGrabed();
	void LeverShouldBeZero();
	void ControlByPlayerHand();
	virtual void LeverTriggered();

private:

	UPROPERTY()
	class AEscapePlayer* player;
	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* hapticFeedback;
	// ���� �̵� ������ġ
	FVector startVector;
	// �÷��̾ ���� ��Ҵ��� ����
	bool bIsGrabed;
	// ���� �̵� ������ġ �ѹ��� ����ϰ�
	bool bRecordOnce;
	bool bTriggerOnce;
};
