// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GrabComponent.h"
#include "HandAnim.generated.h"

UENUM(BlueprintType)
enum class EHandAnimState : uint8
{
	IDLE UMETA(DisplayName = "Idle"),
	GRASP UMETA(DisplayName = "Grasp"),
	POINT UMETA(DisplayName = "Point"),
	THUMBUP UMETA(DisplayName = "ThumbUp"),
	OBJECTHOLD UMETA(DisplayName = "ObjectHold"),
};

UCLASS()
class SPACEESCAPE_API UHandAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HandAnim")
	EHandAnimState animState = EHandAnimState::IDLE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HandAnim")
	EGrabType grabType = EGrabType::NONE;
};
