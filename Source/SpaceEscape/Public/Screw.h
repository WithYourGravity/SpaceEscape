// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Screw.generated.h"

UCLASS()
class SPACEESCAPE_API AScrew : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScrew();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*������Ʈ ����*/
	UPROPERTY(EditDefaultsOnly, Category = "Screw")
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, Category = "Screw")
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category = "Screw")
	class UArrowComponent* arrowComp;
	//UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	//class UGrabComponent* grabComp;

	UPROPERTY()
	class AScrewDriver* driver;

	//����̹��� ���簡 ������
	UFUNCTION()
	void AttachtoDriver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//����̹����� ���簡 ��������
	UFUNCTION()
	void DettachFromDriver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//��Ҵ°�
	bool isAttaching;

	//ȸ���ϴ°�
	bool isRotating;
	void bRotating();

	//���� ���� ��ġ
	FVector startLoc;
	//���簡 ���ø�ŭ �� ���Դ°�
	bool isEnoughRotated;
	void bEnoughCameOut();

	//����̹��� ���� ȸ����
	float prevDriverRot;
	//��ȭ�Ǵ� ��
	float deltaRot;

	//���簡 �̵��Ѵ�
	void MoveScrew();
	//ȸ���� ������ ������ �ݿø�
	int32 rotIntAngle = 0;

	//���簡 �� ���� �Ÿ�
	double screwDist;
	
	//���簡 �� ���Դٸ� �ٴڿ� ��������
	void CameOutScrew();
};
