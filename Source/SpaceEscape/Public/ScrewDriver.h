// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScrewDriver.generated.h"

UCLASS()
class SPACEESCAPE_API AScrewDriver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScrewDriver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Screw")
		class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Screw")
		class UBoxComponent* handleBoxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Screw")
		class UStaticMeshComponent* meshComp;

	//����̹��� �տ� ���δ�
	void AttachedByHand();
	class AEscapePlayer* player;
	class AScrew* attachedScrew;

	//����̹��� ���簡 ������
	UFUNCTION()
		void AttachNailtoScrew(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//����̹��� ���簡 ���� ��
	void AttachNailProcess();
	void ComingoutNails();

	//�� �ʱ�ȭ
	void initScrew();
	bool isAttaching; //ȸ�������� üũ
	float initRot;

	//ȸ���� ��?
	float newRot;
	//��ȭ�Ǵ� ��?
	float deltaRot;

	//ȸ���� �����־��
	void AddRotation();

};
