// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class SPACEESCAPE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	class UProjectileMovementComponent* movementComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* bulletTrailComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UNiagaraSystem* bloodEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UMaterialInstance* bloodDecal;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<class AActor> hitEffect;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnStop(const FHitResult& ImpactResult);

private:
	int32 bulletPower = 3;
};
