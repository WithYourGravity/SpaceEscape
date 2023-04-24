// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorButton.generated.h"

DECLARE_DELEGATE(FOpenDoorDele)

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

	//손이 버튼에 닿으면 문이 열린다
	UFUNCTION()
	void OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void CheckClearStage();

	UFUNCTION()
	void ReportOpen();
	void NoReportOpen();

	//문이 한번 열고 닫히면, 다음 스테이지 클리어 할 때까지 문이 열리지 않도록


public:
	class ADoors* door;

	FOpenDoorDele openDoorDele;

	bool bOpen;

	bool bClose;


};
