// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Doors.generated.h"

UCLASS()
class SPACEESCAPE_API ADoors : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*퍼즐풀기를 완료하면 양 옆으로 열리는 문*/
	UPROPERTY(EditDefaultsOnly, Category = "Door Settings")
	class UStaticMeshComponent* leftDoorMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Door Settings")
	class UStaticMeshComponent* rightDoorMesh;

	//퍼즐을다 풀었는가



};
