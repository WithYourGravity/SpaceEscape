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

	// Input Action for Fire
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_FireLeft;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_FireRight;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class UStaticMeshComponent* gunMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* grabComp;

	UFUNCTION()
	void OnGrabbed();
	UFUNCTION()
	void OnDropped();

	void BindToInput();

	void FireLeft();
	void FireRight();

private:
	// Crosshair
	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ACrosshair> crosshairFactory;

	// Crosshair Instance
	UPROPERTY()
	class ACrosshair* crosshair;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	float crosshairScale = 0.3f;

	// Draw Crosshair
	void DrawCrosshair();
};
