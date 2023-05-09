// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallPad.generated.h"

UCLASS()
class SPACEESCAPE_API AWallPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* wallScreenComp;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY()
	class ARoomManager* rm;
	UPROPERTY()
	class UPuzzleRoomTwoWallPadWidget* gravityWidget;
	bool bActiveOnce;
};
