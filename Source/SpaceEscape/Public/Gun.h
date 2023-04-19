// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputActionValue.h"
#include "Gun.generated.h"

UCLASS()
class SPACEESCAPE_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_WeaponLeft;
	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_WeaponRight;


	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class UStaticMeshComponent* gunMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class UStaticMeshComponent* gunSlideMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* grabComp;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* slideGrabComp;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	class USceneComponent* muzzleLocation;

	UPROPERTY(EditAnywhere, Category = "Fire")
	TSubclassOf<class AActor> bulletFactory;

	UFUNCTION()
	void OnGrabbed();
	UFUNCTION()
	void OnDropped();

	void Fire();

private:
	// Crosshair
	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ACrosshair> crosshairFactory;

	// Crosshair Instance
	UPROPERTY()
	class ACrosshair* crosshair;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	float crosshairScale = 0.3f;

	// Grab
	bool bIsOnGrabbed = false;

	// Fire
	UPROPERTY(EditAnywhere, Category = "Fire", meta = (AllowPrivateAccess = true))
	float fireDistance = 10000.0f;
	
	void DrawCrosshair();
	void GrabSlider();
};
