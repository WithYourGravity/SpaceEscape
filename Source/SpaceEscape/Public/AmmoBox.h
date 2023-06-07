// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"

UCLASS()
class SPACEESCAPE_API AAmmoBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* boxMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* coverMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UGrabComponent* grabComp;

	UFUNCTION()
	void OnGrabed();
	UFUNCTION()
	void OnDroped();

	void BoxShouldBeClosed();
	void ControlByPlayerHand();

private:

	UPROPERTY()
	class AEscapePlayer* player;

	bool bIsGrabed;
	bool bRecordOnce;
	bool bRecordRightOnce;
	bool bRecordLeftOnce;
	FVector startVector;
	FVector afterVector;
	FVector deltaVector;
};
