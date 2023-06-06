// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceShipJoystick.generated.h"

UCLASS()
class SPACEESCAPE_API ASpaceShipJoystick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceShipJoystick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* sceneComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* baseMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* stickMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UGrabComponent* grabComp;
	UPROPERTY(EditDefaultsOnly)
	class UPhysicsConstraintComponent* constraintComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* stickPosComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* forwardPosComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* backPosComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* leftPosComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* rightPosComp;

	UFUNCTION()
	void ChangeIsGrabed();
	UFUNCTION()
	void StickOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void StickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void ControlByPlayerHand();
	void RotateFunction(char componentName);

private:
	UPROPERTY()
	class AEscapePlayer* player;
	UPROPERTY()
	class UHapticFeedbackEffect_Base* hapticFeedback;
	UPROPERTY()
	class UMotionControllerComponent* playerHand;
	UPROPERTY()
	class ASpaceShip* ship;
	UPROPERTY()
	class ARoomManager* rm;

	FTimerHandle stickHandle;
	char otherCompNameForTimer;
	bool bIsGrabed;
	bool bPlaySeqOnce;
};
