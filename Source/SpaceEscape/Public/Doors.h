// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Doors.generated.h"

class UCurveFloat;

UCLASS()
class SPACEESCAPE_API ADoors : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*����Ǯ�⸦ �Ϸ��ϸ� �� ������ ������ ��*/
	UPROPERTY(EditDefaultsOnly, Category = "Door Settings")
	class UStaticMeshComponent* doorMesh;

	//�ϴ� trigger box�� ������ ��������
	UPROPERTY(EditDefaultsOnly, Category="DoorSettings")
	class UBoxComponent* triggerboxComp;

	UFUNCTION()
	void OnTriggeredOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggeredEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void Open();
	UFUNCTION()
	void Close();

	/*���� �ε巯�� �̵�*/
	UFUNCTION()
	void TimeLineProgress(float val);

	FTimeline curveTimeline;
	UPROPERTY()
	FVector startPoint;
	UPROPERTY()
	FVector endPoint;
	UPROPERTY(EditDefaultsOnly, Category = "DoorTimeLine")
	float yOffset = 200;
	UPROPERTY(EditDefaultsOnly, Category = "DoorTimeLine")
	UCurveFloat* curveFloat;
	//�� ���� �浹���� �������ΰ�
	bool bIsOpenOverlaping = true;
	//�� ���� ��ġ
	FVector initLoc;

	//������ �� Ǯ���°�

};

