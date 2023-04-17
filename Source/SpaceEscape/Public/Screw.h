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
	UPROPERTY(EditDefaultsOnly, Category = "Nail")
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Nail")
	class UStaticMeshComponent* meshComp;

	//드라이버를 손에 붙인다
	void AttachedByHand();
	class AEscapePlayer* player;
	

};
