// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"

UCLASS()
class SPACEESCAPE_API AMagazine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagazine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class UStaticMeshComponent* magazineMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class UStaticMeshComponent* bulletMeshComp1;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	class UStaticMeshComponent* bulletMeshComp2;

	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	class UGrabComponent* grabComp;

	UPROPERTY()
	class AGun* gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	int32 maxBulletCount = 25;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnGrabbed();
	UFUNCTION()
	void OnDropped();

	FORCEINLINE int32 GetCurrentBulletCount() const { return currentBulletCount; }
	FORCEINLINE void FireBullet() { currentBulletCount--; }

private:
	bool bIsOverlapGun = false;

	int32 currentBulletCount;

	float grabValue = 0.0f;
	bool bIsLoadingMagazine = false;

	void GrabMagazine();
};
