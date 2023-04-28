// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "QuatRotatedDoor.generated.h"

UCLASS()
class SPACEESCAPE_API AQuatRotatedDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuatRotatedDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*퍼즐풀기를 완료하면 옆으로 열리는 문*/
	UPROPERTY(EditDefaultsOnly, Category = "Door Settings")
		class UStaticMeshComponent* doorMesh;

	UFUNCTION()
		void Open();
	UFUNCTION()
		void Close();

	//문 여는 충돌감지 진행중인가
	bool bIsOpenOverlaping = true;
	UFUNCTION()
		void ChangeDoorOverlaping();

	/*문의 부드러운 이동*/
	UFUNCTION()
		void TimeLineProgress(float val);

	FTimeline curveTimeline;
	UPROPERTY()
		FVector startPoint;
	UPROPERTY()
		FVector endPoint;
	UPROPERTY(EditDefaultsOnly, Category = "Door Settings")
		float xOffset = 200;
	UPROPERTY(EditDefaultsOnly, Category = "Door Settings")
		UCurveFloat* curveFloat;

	//문 최초 위치
	FVector initLoc;

};
