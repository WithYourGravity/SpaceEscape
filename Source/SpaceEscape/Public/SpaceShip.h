// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceShip.generated.h"

UCLASS()
class SPACEESCAPE_API ASpaceShip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* shipMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* forLocComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* forJoyLocComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* morseCodeTableComp;
	UPROPERTY(EditDefaultsOnly)
	class UChildActorComponent* morseScreenActorComp;
	UPROPERTY(EditDefaultsOnly)
	class UChildActorComponent* morseLeverActorComp;
	UPROPERTY(EditDefaultsOnly)
	class UChildActorComponent* morseButtonActorComp;
	UPROPERTY(EditDefaultsOnly)
	class UChildActorComponent* rankingActorComp;
	UPROPERTY(EditDefaultsOnly)
	class UBillboardComponent* billboardComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* earthStickerComp;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapJoystick(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BoardingShip();
	UFUNCTION()
	void SpawnControlableJoystick();

private:

	UPROPERTY()
	class AEscapePlayer* player;
	UPROPERTY()
	class UStaticMeshComponent* stickComp;
	UPROPERTY()
	class ASpaceShipJoystick* controlableStick;
	UPROPERTY()
	class ARoomManager* rm;

	FTimerHandle stickDownHandle;
	FVector stickLoc;
	FVector stickFinalLoc;
	bool bPlayerUseTeleport;
	bool bIsBoarding;
	bool bReadyToBoarding;
	float stickTimer;

};
