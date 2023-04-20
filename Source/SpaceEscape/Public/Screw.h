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

	/*컴포넌트 생성*/
	UPROPERTY(EditDefaultsOnly, Category = "Nail")
		class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Nail")
		class UStaticMeshComponent* meshComp;
	/*
	UFUNCTION()
	void AttachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DettachScrewToNail(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//못이 다 나오면 바닥으로 떨어지도록

	void AttachNailProcess();
*/
	int32 comeOutNailDist = 5; //나사 한번 돌렸을 때의 이동 거리		
	int32 rotCount; //나사 한바퀴 돌린 횟수

	class AEscapePlayer* player;

	//회전한 값
	double processedRot = 5;
	FRotator initializedRot;

	//screw값 초기화
	void initScrewDriver();
	bool isAttaching; //회전중인지 체크
	class AScrew* attachedScrew;
	//FRotator initNailRot;
	double initRot;

};
