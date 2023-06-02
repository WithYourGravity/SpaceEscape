// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ResearcherEnemy.generated.h"

UCLASS()
class SPACEESCAPE_API AResearcherEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AResearcherEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	class UEnemyFSM* enemyFSM;

	UPROPERTY(EditDefaultsOnly)
	class UNavigationInvokerComponent* navInvokerComp;

	// Sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* attackSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* damageSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* moveSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* dieSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* knockSoundCue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class UAudioComponent* audioComp;
};
