// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Nail.generated.h"

UCLASS()
class SPACEESCAPE_API ANail : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*������Ʈ ����*/
	UPROPERTY(EditDefaultsOnly, Category="Nail")
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category="Nail")
	class UStaticMeshComponent* meshComp;

	UFUNCTION()
		void AttachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	int32 comeOutNailDist = 5; //���� �ѹ� ������ ���� �̵� �Ÿ�		
	int32 rotCount; //���� �ѹ��� ���� Ƚ��

	void initScrew();

	bool isAttacing;
	class AScrew* attachedScrew;
	FRotator initNailRot;
	double initPitch;
};
