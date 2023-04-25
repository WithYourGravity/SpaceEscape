// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"

UCLASS()
class SPACEESCAPE_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Ladder")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Ladder")
	class UGrabComponent* grabComp1;
	UPROPERTY(EditDefaultsOnly, Category = "Ladder")
	class UGrabComponent* grabComp2;
	UPROPERTY(EditDefaultsOnly, Category = "Ladder")
	class UGrabComponent* grabComp3;
	UPROPERTY(EditDefaultsOnly, Category = "Ladder")
	class UGrabComponent* grabComp4;
	UPROPERTY(EditDefaultsOnly, Category = "Ladder")
	class UGrabComponent* grabComp5;
	UPROPERTY(EditDefaultsOnly, Category = "Ladder")
	class UGrabComponent* grabComp6;
	UPROPERTY(EditDefaultsOnly, Category = "Ladder")
	class UGrabComponent* grabComp7;

};
