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

	// ��� �Լ�
	bool TryGrab(class UMotionControllerComponent* motionController);

	// ���� �Լ�
	bool TryRelease();

	void SetShouldSimulateOnDrop();

	// Object Simulate Physics ���� �Լ�
	void SetPrimitiveCompPhysics(bool simulate);

	// ��� ������ Ȯ���ϴ� �Լ�
	EControllerHand GetHeldByHand();

	// motionController �� parentObject ���̱�
	bool AttachParentToMotionController(class UMotionControllerComponent* motionController);

	// parentObject ���� �� Ÿ��
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
