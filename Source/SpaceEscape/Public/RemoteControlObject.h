// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RemoteControlObject.generated.h"

UCLASS()
class SPACEESCAPE_API ARemoteControlObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARemoteControlObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly)
	class USceneCaptureComponent2D* cameraCaptureComp;

	void ControlManager(FString input);
	void MoveForward();
	void MoveBackward();
	void TurnRight();
	void TurnLeft();
	void StopMove();
	void StopTurn();

	FTimerHandle moveHandle;
	FTimerHandle turnHandle;


private:
	float moveSpeed = 400;
	float turnSpeed = 100;
};
