// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorButton.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOpenDoorDele)

UCLASS()
class SPACEESCAPE_API ADoorButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Door Button Settings")
	class UBoxComponent*boxComp;
	UPROPERTY(EditDefaultsOnly, Category = "Door Button Settings")
	class UStaticMeshComponent* buttonMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Door Button Settings")
	TArray<class AActor*> outDoorActors;


	//손이 버튼에 닿으면 문이 열린다
	UFUNCTION()
	void OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CheckClearStage();
	//UFUNCTION()
	//void ReportOpen();

	void bOpenDoor();
	void OpenAllDoors();
	void CloseAllDoors();

	UPROPERTY()
	TArray<AActor*> doors;
public:
	//FOpenDoorDele openDoorDele;
	//델리게이트 받으면 버튼 활성화 되도록
	bool bCanButtonClicked;
	//문이 한번 열고 닫혔었는지
	bool bOpened;

private:
	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* hapticFeedback;

};
