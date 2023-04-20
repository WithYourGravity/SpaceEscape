// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LeverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEESCAPE_API ULeverComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULeverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
	bool GetIsTriggered();

private:

	UPROPERTY()
	class AEscapePlayer* player;
	// 레버 이동 시작위치
	FVector startVector;
	// 플레이어가 레버 잡았는지 여부
	bool bIsGrabed;
	// 레버 이동 시작위치 한번만 기록하게
	bool bDoOnce;
	// 레버 작동 되었는지
	bool bIsTriggered;
};
