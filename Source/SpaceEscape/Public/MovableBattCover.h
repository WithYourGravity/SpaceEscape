// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovableCover.h"
#include "MovableBattCover.generated.h"

DECLARE_DELEGATE(FAllowBattAttatch)
/**
 * 
 */
UCLASS()
class SPACEESCAPE_API AMovableBattCover : public AMovableCover
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovableBattCover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//���͸� Ŀ��
	//���ǥ��plane
	UPROPERTY(EditDefaultsOnly, Category="Battery Cover")
	class UStaticMeshComponent* warningSignMesh;
	//������mesh
	UPROPERTY(EditDefaultsOnly, Category="Battery Cover")
	class UStaticMeshComponent* handleMesh;

	FAllowBattAttatch allowBatt;


	virtual void FallingCover() override;
};
