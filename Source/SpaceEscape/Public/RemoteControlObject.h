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
	UPROPERTY(EditDefaultsOnly)
	class UGrabComponent* grabComp;

	void ControlManager(FString input);
	void MoveForward();
	void MoveBackward();
	void TurnRight();
	void TurnLeft();
	void StopMove();
	void StopTurn();
	void CheckIsUp();

	FTimerHandle moveHandle;
	FTimerHandle turnHandle;
	FTimerHandle upCheckhandle;

private:
	float moveSpeed = 80;
	float turnSpeed = 60;
	bool bcanMove;
};
