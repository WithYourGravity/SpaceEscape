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

	//드라이버를 손에 붙인다
	void AttachedByHand();
	class AEscapePlayer* player;
	class AScrew* attachedScrew;

	//드라이버에 나사가 닿으면
	UFUNCTION()
		void AttachNailtoScrew(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//드라이버에 나사가 닿은 후
	void AttachNailProcess();
	void ComingoutNails();

	//값 초기화
	void initScrew();
	bool isAttaching; //회전중인지 체크
	float initRot;

	//회전된 값?
	float newRot;
	//변화되는 값?
	float deltaRot;

	//회전을 더해주어라
	void AddRotation();

};
