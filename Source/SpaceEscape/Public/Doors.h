// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Doors.generated.h"

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
	class UStaticMeshComponent* leftDoorMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Door Settings")
	class UStaticMeshComponent* rightDoorMesh;

	UPROPERTY(EditDefaultsOnly)
	float slideDist = 30;

	//�ϴ� trigger box�� ������ ��������
	UPROPERTY(EditDefaultsOnly, Category="DoorSettings")
	class UBoxComponent* triggerboxComp;

	UFUNCTION()
	void OnTriggeredOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggeredEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//������ �� Ǯ���°�

	void Open();
	void Close();


};

