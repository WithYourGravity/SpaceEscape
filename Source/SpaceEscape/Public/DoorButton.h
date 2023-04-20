// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorButton.generated.h"

DECLARE_DELEGATE(FOpenDoorDele)

UCLASS()
class SPACEESCAPE_API ADoorButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Door Button Settings")
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, Category = "Door Button Settings")
	class UStaticMeshComponent* buttonMesh;

	//손이 버튼에 닿으면 문이 열린다
	UFUNCTION()
	void OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//퀴즈가 다 풀렸다고 dele 받으면 충돌이 활성화된다.

	//퍼즐을 다 풀었는가


public:
	class ADoors* door;
	class AEscapePlayer* player;
	FOpenDoorDele openDoorDele;

};
