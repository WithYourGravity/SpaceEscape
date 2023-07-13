// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interactiveObj.generated.h"

UCLASS()
class SPACEESCAPE_API AinteractiveObj : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AinteractiveObj();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "ObjSettings")
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, Category = "ObjSettings")
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category = "ObjSettings")
	class UGrabComponent* grabComp;

};
