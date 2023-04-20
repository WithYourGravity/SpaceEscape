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
	// 레버 이동 시작위치
	FVector startVector;
	// 플레이어가 레버 잡았는지 여부
	bool bIsGrabed;
	// 레버 이동 시작위치 한번만 기록하게
	bool bRecordOnce;
	bool bTriggerOnce;
};
