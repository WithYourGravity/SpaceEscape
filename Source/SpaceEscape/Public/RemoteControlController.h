// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RemoteControlController.generated.h"

UCLASS()
class SPACEESCAPE_API ARemoteControlController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARemoteControlController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* tabletMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* buttonGoMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* buttonBackMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* buttonLeftMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* buttonRightMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* buttonGoCollision;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* buttonBackCollision;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* buttonLeftCollision;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* buttonRightCollision;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* tabletScreenComp;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY()
	class ARemoteControlObject* remoteObject;
	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* hapticFeedback;

};
	