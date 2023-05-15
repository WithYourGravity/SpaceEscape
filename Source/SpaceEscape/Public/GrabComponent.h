// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GrabComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGrabbedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDroppedDelegate)

UENUM(BlueprintType)
enum class EGrabType : uint8
{
	NONE UMETA(DisplayName = "None"),
	FREE UMETA(DisplayName = "Free"),
	SNAP UMETA(DisplayName = "Snap"),
	LEVER UMETA(DisplayName = "Lever"),
	GUNSLIDER UMETA(DisplayName = "GunSlider"),
	CLIMB UMETA(DisplayName = "Climb"),
	MARKER UMETA(DisplayName = "Marker"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEESCAPE_API UGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 잡기 함수
	bool TryGrab(class UMotionControllerComponent* motionController);

	// 놓기 함수
	bool TryRelease();

	void SetShouldSimulateOnDrop();

	// Object Simulate Physics 설정 함수
	void SetPrimitiveCompPhysics(bool simulate);

	// 어느 손인지 확인하는 함수
	EControllerHand GetHeldByHand();

	// motionController 에 parentObject 붙이기
	bool AttachParentToMotionController(class UMotionControllerComponent* motionController);

	// parentObject 잡을 때 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab")
	EGrabType grabType = EGrabType::FREE;

	bool bSimulateOnDrop = false;

	bool bIsGunSlideGrabbed = false;

	FOnGrabbedDelegate onGrabbedDelegate;
	FOnDroppedDelegate onDroppedDelegate;

	bool bIsHeld = false;

	FVector grabLocation;

private:
	UPROPERTY()
	class UMotionControllerComponent* motionControllerComp;

	UPROPERTY()
	class UHapticFeedbackEffect_Base* grabHapticEffect;

};
