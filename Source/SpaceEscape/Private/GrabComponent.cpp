// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"
#include "MotionControllerComponent.h"
#include "Haptics/HapticFeedbackEffect_Base.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> tempHapticEffect(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/VRTemplate/Haptics/GrabHapticEffect.GrabHapticEffect'"));
	if (tempHapticEffect.Succeeded())
	{
		grabHapticEffect = tempHapticEffect.Object;
	}
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	SetShouldSimulateOnDrop();
	
	// Parent Object 물리기능 활성화
	SetPrimitiveCompPhysics(true);
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

bool UGrabComponent::TryGrab(UMotionControllerComponent* motionController)
{
	switch (grabType)
	{
	// 자유롭게 잡기
	case EGrabType::FREE:
		SetPrimitiveCompPhysics(false);

		if (AttachParentToMotionController(motionController))
		{
			bIsHeld = true;
		}
		break;
	// 정해진 곳 잡기
	case EGrabType::SNAP:
		SetPrimitiveCompPhysics(false);

		if (!AttachParentToMotionController(motionController))
		{
			return false;
		}
		bIsHeld = true;

		// grab component 의 rotation, location 설정
		GetAttachParent()->SetRelativeRotation(GetRelativeRotation().GetInverse(), false, nullptr, ETeleportType::TeleportPhysics);

		{
		FVector newLocation = motionController->GetComponentLocation();
		newLocation += (GetComponentLocation() - GetAttachParent()->GetComponentLocation()) * -1.0f;
		GetAttachParent()->SetWorldLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics);
		}

		break;
	default:
		break;
	}

	if (!bIsHeld)
	{
		return false;
	}

	motionControllerComp = motionController;

	// grabbed delegate
	if (onGrabbedDelegate.IsBound())
	{
		onGrabbedDelegate.Execute();
	}

	// haptic effect
	if (grabHapticEffect)
	{
		GetWorld()->GetFirstPlayerController()->PlayHapticEffect(grabHapticEffect, GetHeldByHand());
	}

	return true;
}

bool UGrabComponent::TryRelease()
{
	switch (grabType)
	{
	case EGrabType::FREE:
	case EGrabType::SNAP:
		if (bSimulateOnDrop)
		{
			SetPrimitiveCompPhysics(true);
		}
		else
		{
			GetAttachParent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}
		
		bIsHeld = false;
		break;
	default:
		break;
	}

	if (bIsHeld)
	{
		return false;
	}

	// On Dropped Delegate
	if (onDroppedDelegate.IsBound())
	{
		onDroppedDelegate.Execute();
	}

	return true;
}

void UGrabComponent::SetShouldSimulateOnDrop()
{
	if (Cast<UPrimitiveComponent>(GetAttachParent())->IsAnySimulatingPhysics())
	{
		bSimulateOnDrop = true;
	}
}

void UGrabComponent::SetPrimitiveCompPhysics(bool simulate)
{
	Cast<UPrimitiveComponent>(GetAttachParent())->SetSimulatePhysics(simulate);
}

EControllerHand UGrabComponent::GetHeldByHand()
{
	return motionControllerComp->MotionSource == FName("Left") ? EControllerHand::Left : EControllerHand::Right;
}

bool UGrabComponent::AttachParentToMotionController(UMotionControllerComponent* motionController)
{
	return GetAttachParent()->AttachToComponent(motionController, FAttachmentTransformRules::KeepWorldTransform);
}

