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
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* stickMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* baseMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UGrabComponent* grabComp;

	UFUNCTION()
	void ChangeIsGrabed();
	void ControlByPlayerHand();
	void SetStickDefault();


private:

	UPROPERTY()
	class AEscapePlayer* player;

	float alpha;
	bool bNeedToSetDefault;
	bool bIsGrabed;
};
