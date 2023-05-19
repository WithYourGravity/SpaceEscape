// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleRoomThreeJoystick.generated.h"

UCLASS()
class SPACEESCAPE_API APuzzleRoomThreeJoystick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleRoomThreeJoystick();

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
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* resetButtonComp;


	UFUNCTION()
	void ChangeIsGrabed();
	UFUNCTION()
	void StickOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void StickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void ResetButtonOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BreakConstraintWhenClear();
	void ControlByPlayerHand();
	void GiveHapticFeedback();
	void MoveFunction(char componentName);

private:

	UPROPERTY()
	class AEscapePlayer* player;
	UPROPERTY()
	class APuzzleRoomThreePathFinding* puzzlePathFinding;
	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* hapticFeedback;
	UPROPERTY()
	class UMotionControllerComponent* playerHand;
	FTimerHandle stickHandle;
	char otherCompNameForTimer;
	bool bIsGrabed;
};
