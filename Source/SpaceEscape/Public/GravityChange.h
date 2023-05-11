// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravityChange.generated.h"

UCLASS()
class SPACEESCAPE_API AGravityChange : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravityChange();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditDefaultsOnly)
	//class UBoxComponent* boxComp;

	//UPROPERTY(EditDefaultsOnly, Category = "Gravity")
	//class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	bool bIsZeroGravity = false;

	UPROPERTY()
	class ARoomManager* roomManager;

	UPROPERTY()
	class ADoorButton* doorButton;
	//UPROPERTY()
	//class AEscapePlayer* player;

	UPROPERTY()
	class AWorldSettings* worldSettings;

	void ChangeGravity(bool bCurGravity);

	void ChangeStageTwoGravity();

	//UFUNCTION()
	//void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
