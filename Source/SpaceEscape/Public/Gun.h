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


	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class UStaticMeshComponent* gunMeshComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class UStaticMeshComponent* gunSlideMeshComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class UStaticMeshComponent* gunTriggerMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class USceneComponent* triggerSceneComp;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* grabComp;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* slideGrabComp;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	class USceneComponent* muzzleLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
	class UBoxComponent* magazineBoxComp;

	UPROPERTY(EditAnywhere, Category = "Fire")
	TSubclassOf<class ABullet> bulletFactory;
	
	UPROPERTY()
	class AMagazine* magazine;

	UPROPERTY()
	class AEscapePlayer* player;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* muzzleFlashComp;

	bool bIsOnGrabbed = false;
	bool bOnReloading = false;
	bool bDoReloading = false;
	bool bFireCompleted = false;

	UFUNCTION()
	void OnGrabbed();
	UFUNCTION()
	void OnDropped();

	void Fire(float fireAlpha);
	void DropMagazine();

private:
	// Crosshair
	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ACrosshair> crosshairFactory;

	UPROPERTY()
	class ACrosshair* crosshair;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	float crosshairScale = 0.2f;
	
	UPROPERTY(EditAnywhere, Category = "Fire", meta = (AllowPrivateAccess = true))
	float fireDistance = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Fire", meta = (AllowPrivateAccess = true))
	float reloadSliderDistance = 25.0f;

	FVector initGunSlideCompLocation;
	FVector initGunSlideMeshLocation;

	FTimerHandle magazineTimer;

	float currentTime = 0.0f;
	float dropMagazineTime = 0.2f;

	UPROPERTY()
	class UHapticFeedbackEffect_Base* grabHapticEffect;

	void DrawCrosshair();
	void GrabSlider();

	void OpenSlider();
	void CloseSlider();
};
