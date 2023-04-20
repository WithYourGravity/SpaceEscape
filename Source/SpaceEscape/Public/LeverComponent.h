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
	// ���� �̵� ������ġ
	FVector startVector;
	// �÷��̾ ���� ��Ҵ��� ����
	bool bIsGrabed;
	// ���� �̵� ������ġ �ѹ��� ����ϰ�
	bool bDoOnce;
	// ���� �۵� �Ǿ�����
	bool bIsTriggered;
};
