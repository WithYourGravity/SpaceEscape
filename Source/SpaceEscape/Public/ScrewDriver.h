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

	UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	class UGrabComponent* grabComp;
	UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	class UArrowComponent* arrowComp;

	//드라이버에 나사가 닿으면
	UFUNCTION()
	void AttachtoScrew(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//드라이버에서 나사가 떨어지면
	UFUNCTION()
	void DettachFromScrew(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//닿았는가
	bool isAttaching;	

	//드라이버의 실시간 회전값
	float driverRot;
	//드라이버의 이전 회전값
	float prevDriverRot;
	//변화되는 값?
	float deltaRot;

};
