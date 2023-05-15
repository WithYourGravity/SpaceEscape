// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapePlayer.h"
#include "Crosshair.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "GrabComponent.h"
#include "Gun.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Components/SphereComponent.h"

// Sets default values
AEscapePlayer::AEscapePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(73.0f);

	vrCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	vrCamera->SetupAttachment(RootComponent);
	vrCamera->bUsePawnControlRotation = false;
	vrCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 35.0f));

	// MotionController
	leftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	leftHand->SetupAttachment(RootComponent);
	leftHand->SetTrackingMotionSource(FName("Left"));

	rightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	rightHand->SetupAttachment(RootComponent);
	rightHand->SetTrackingMotionSource(FName("Right"));

	leftAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("leftAim"));
	leftAim->SetupAttachment(RootComponent);
	leftAim->SetTrackingMotionSource(FName("LeftAim"));

	rightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("rightAim"));
	rightAim->SetupAttachment(RootComponent);
	rightAim->SetTrackingMotionSource(FName("RightAim"));

	// Hand Mesh
	// Left
	leftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	leftHandMesh->SetupAttachment(leftHand);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempLeftMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (tempLeftMesh.Succeeded())
	{
		leftHandMesh->SetSkeletalMesh(tempLeftMesh.Object);
		leftHandMesh->SetRelativeLocation(FVector(-2.9f, -3.5f, 4.5f));
		leftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
	}
	// Right
	rightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	rightHandMesh->SetupAttachment(rightHand);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRightMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (tempRightMesh.Succeeded())
	{
		rightHandMesh->SetSkeletalMesh(tempRightMesh.Object);
		rightHandMesh->SetRelativeLocation(FVector(-2.9f, 3.5f, 4.5f));
		rightHandMesh->SetRelativeRotation(FRotator(25, 0, 90));
	}

	// Right Index Finger Collision
	rightIndexFingerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("rightIndexFingerCollision"));
	rightIndexFingerCollision->SetupAttachment(rightHandMesh, FName("indexCollision_r"));
	rightIndexFingerCollision->SetCollisionProfileName(FName("FingerPreset"));
	rightIndexFingerCollision->SetSphereRadius(0.5f);
	// Left Index Finger Collision
	leftIndexFingerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("leftIndexFingerCollision"));
	leftIndexFingerCollision->SetupAttachment(leftHandMesh, FName("indexCollision_l"));
	leftIndexFingerCollision->SetCollisionProfileName(FName("FingerPreset"));
	leftIndexFingerCollision->SetSphereRadius(0.5f);

	// Teleport
	teleportCircle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("teleportCircle"));
	teleportCircle->SetupAttachment(RootComponent);
	teleportCircle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	teleportCircle->SetVisibility(false);

	// Teleport Curve Niagara
	teleportCurveComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("teleportCurveComp"));
	teleportCurveComp->SetupAttachment(RootComponent);
	teleportCurveComp->SetVisibility(false);
}

// Called when the game starts or when spawned
void AEscapePlayer::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced Input ���ó��
	auto PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		auto localPlayer = PC->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_PlayerInput, 0);
			subSystem->AddMappingContext(IMC_Hand, 0);
		}
	}

	// HMD �� ����Ǿ� �ִٸ�
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		// �⺻ tracking offset ����
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	}
	// HMD �� ����Ǿ� ���� �ʴٸ�
	else
	{
		// Hand �� �׽�Ʈ�� �� �ִ� ��ġ�� �̵�
		rightHand->SetRelativeLocation(FVector(20, 20, 0));
		rightAim->SetRelativeLocation(FVector(20, 20, 0));
		// ī�޶��� UsePawnControlRotation Ȱ��ȭ
		vrCamera->bUsePawnControlRotation = true;
	}

	// moveMode �� TELEPORT �̸� �ڷ���Ʈ ��� �ʱ�ȭ
	if (moveMode == EMoveModeState::TELEPORT)
	{
		ResetTeleport();
	}

	currentLocation = GetActorLocation();
}

// Called every frame
void AEscapePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// HMD �� ����Ǿ� ���� ������
	if (!UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		// ���� ī�޶� ����� ��ġ�ϵ��� �Ѵ�.
		rightHand->SetRelativeRotation(vrCamera->GetRelativeRotation());
		rightAim->SetRelativeRotation(vrCamera->GetRelativeRotation());
	}

	if (moveMode == EMoveModeState::TELEPORT)
	{
		if (bTeleporting)
		{
			DrawTeleportCurve();
		}

		if (teleportCurveComp)
		{
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(teleportCurveComp, FName("User.PointArray"), linePoints);
		}
	}

	if (bIsClimbing)
	{
		currentLocation = FMath::VInterpTo(currentLocation, GetActorLocation(), DeltaTime, 1.0f);

		if (bIsGrabbedLeft && heldComponentLeft && heldComponentLeft->grabType == EGrabType::CLIMB)
		{
			FVector moveLocation = heldComponentLeft->grabLocation - leftHand->GetComponentLocation();
			
			SetActorLocation(GetActorLocation() + moveLocation);
			
			leftHand->SetWorldLocation(heldComponentLeft->grabLocation);
		}
		if (bIsGrabbedRight && heldComponentRight && heldComponentRight->grabType == EGrabType::CLIMB)
		{
			FVector moveLocation = heldComponentRight->grabLocation - rightHand->GetComponentLocation();
			
			SetActorLocation(GetActorLocation() + moveLocation);
			
			rightHand->SetWorldLocation(heldComponentRight->grabLocation);
		}
	}
}

// Called to bind functionality to input
void AEscapePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto InputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputSystem)
	{
		// Binding
		InputSystem->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AEscapePlayer::Move);
		InputSystem->BindAction(IA_Mouse, ETriggerEvent::Triggered, this, &AEscapePlayer::Turn);
		InputSystem->BindAction(IA_Teleport, ETriggerEvent::Started, this, &AEscapePlayer::TeleportStart);
		InputSystem->BindAction(IA_Teleport, ETriggerEvent::Completed, this, &AEscapePlayer::TeleportEnd);
		InputSystem->BindAction(IA_GrabLeft, ETriggerEvent::Started, this, &AEscapePlayer::TryGrabLeft);
		InputSystem->BindAction(IA_GrabLeft, ETriggerEvent::Completed, this, &AEscapePlayer::UnTryGrabLeft);
		InputSystem->BindAction(IA_GrabRight, ETriggerEvent::Started, this, &AEscapePlayer::TryGrabRight);
		InputSystem->BindAction(IA_GrabRight, ETriggerEvent::Completed, this, &AEscapePlayer::UnTryGrabRight);
		InputSystem->BindAction(IA_FireLeft, ETriggerEvent::Triggered, this, &AEscapePlayer::Fire);
		InputSystem->BindAction(IA_FireLeft, ETriggerEvent::Completed, this, &AEscapePlayer::FireCompleted);
		InputSystem->BindAction(IA_FireRight, ETriggerEvent::Triggered, this, &AEscapePlayer::Fire);
		InputSystem->BindAction(IA_FireRight, ETriggerEvent::Completed, this, &AEscapePlayer::FireCompleted);
		InputSystem->BindAction(IA_DropMagazineLeft, ETriggerEvent::Started, this, &AEscapePlayer::DropMagazine);
		InputSystem->BindAction(IA_DropMagazineRight, ETriggerEvent::Started, this, &AEscapePlayer::DropMagazine);
	}
	
}

void AEscapePlayer::Move(const FInputActionValue& values)
{
	if (moveMode == EMoveModeState::TELEPORT)
	{
		return;
	}

	FVector2D axis = values.Get<FVector2D>();
	// HMD �� ����Ǿ� ���� �ʴٸ�
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		AddMovementInput(GetActorForwardVector(), axis.X);
		AddMovementInput(GetActorRightVector(), axis.Y);
	}
	// ���� HMD �� ����Ǿ� �ִٸ�
	else
	{
		AddMovementInput(vrCamera->GetForwardVector(), axis.X);
		AddMovementInput(vrCamera->GetRightVector(), axis.Y);
	}
}

void AEscapePlayer::Turn(const FInputActionValue& values)
{
	FVector2D axis = values.Get<FVector2D>();
	AddControllerYawInput(axis.X);
	AddControllerPitchInput(axis.Y);
}

void AEscapePlayer::TeleportStart(const FInputActionValue& values)
{
	if (moveMode == EMoveModeState::JOYSTICK)
	{
		return;
	}

	bTeleporting = true;
	teleportCurveComp->SetVisibility(true);
}

void AEscapePlayer::TeleportEnd(const FInputActionValue& values)
{
	if (moveMode == EMoveModeState::JOYSTICK)
	{
		return;
	}

	// �ڷ���Ʈ�� �Ұ����ϴٸ�
	if (!ResetTeleport())
	{
		// ���� ó���� ���� �ʴ´�.
		return;
	}

	// �ڷ���Ʈ�� �����ϴٸ� �ڷ���Ʈ ��ġ�� �̵�
	SetActorLocation(teleportLocation + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
}

bool AEscapePlayer::ResetTeleport()
{
	// teleportCircle �� Ȱ��ȭ�Ǿ� ���� ���� �ڷ���Ʈ ����
	bool bCanTeleport = teleportCircle->GetVisibleFlag();

	// teleportCircle �Ⱥ��̰� �ϱ�
	teleportCircle->SetVisibility(false);
	bTeleporting = false;
	teleportCurveComp->SetVisibility(false);

	return bCanTeleport;
}

bool AEscapePlayer::CheckHitTeleport(FVector lastPos, FVector& curPos)
{
	// �� �� ���̿� �浹ü �ִ��� Ȯ��
	FHitResult hitInfo;
	bool bHit = HitTest(lastPos, curPos, hitInfo);
	// ���� �ε��� ����� �ٴ��̶��
	if (bHit && hitInfo.GetActor()->GetActorNameOrLabel().Contains(TEXT("Floor")))
	{
		// endPos �� �ε��� ������ ����
		curPos = hitInfo.Location;
		// teleportCircle Ȱ��ȭ
		teleportCircle->SetVisibility(true);
		// teleportCircle ��ġ
		teleportCircle->SetWorldLocation(curPos);
		teleportLocation = curPos;
	}
	else
	{
		teleportCircle->SetVisibility(false);
	}
	return bHit;
}

bool AEscapePlayer::HitTest(FVector lastPos, FVector curPos, FHitResult& hitInfo)
{
	FCollisionQueryParams params;
	// �ڱ��ڽ� ����
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, lastPos, curPos, ECC_Visibility, params);

	return bHit;
}

void AEscapePlayer::DrawTeleportCurve()
{
	// linePoints �ʱ�ȭ
	linePoints.Empty();

	// ����ü ������
	FVector pos = leftAim->GetComponentLocation();
	FVector dir = leftAim->GetForwardVector() * curvedPower;

	// �������� ���� ���� ���
	linePoints.Add(pos);

	for (int32 i = 0; i < lineSmooth; i++)
	{
		// ���� �� ���
		FVector lastPos = pos;

		// ����ü �̵�
		// v = v0 + at
		dir += FVector::UpVector * gravity * simulatedTime;
		// P = P0 + vt
		pos += dir * simulatedTime;

		// ���� �� ���̿� ��ü�� ���θ��� �ִٸ�
		if (CheckHitTeleport(lastPos, pos))
		{
			// �� ���� ������ ������ �Ѵ�.
			linePoints.Add(pos);
			break;
		}

		// ����ü ��ġ ���
		linePoints.Add(pos);
	}
}

void AEscapePlayer::TryGrabLeft()
{
	UGrabComponent* grabComp = GetGrabComponentNearMotionController(leftHand);

	// ���� ��Ҵٸ�
	if (bIsGrabbedLeft && grabComp)
	{
		if (grabComp->TryGrab(leftHand))
		{
			heldComponentLeft = grabComp;

			if (heldComponentLeft == heldComponentRight)
			{
				heldComponentRight = nullptr;
			}
		}
	}
}

void AEscapePlayer::TryGrabRight()
{
	UGrabComponent* grabComp = GetGrabComponentNearMotionController(rightHand);

	// ���� ��Ҵٸ�
	if (bIsGrabbedRight && grabComp)
	{
		if (grabComp->TryGrab(rightHand))
		{
			heldComponentRight = grabComp;

			if (heldComponentRight == heldComponentLeft)
			{
				heldComponentLeft = nullptr;
			}
		}
	}
}

void AEscapePlayer::UnTryGrabLeft()
{
	if (!bIsGrabbedLeft)
	{
		return;
	}

	// ���� ���� ���·� ��ȯ
	bIsGrabbedLeft = false;

	if (heldComponentLeft)
	{
		if (heldComponentLeft->TryRelease())
		{
			heldComponentLeft = nullptr;
		}
	}
}

void AEscapePlayer::UnTryGrabRight()
{
	if (!bIsGrabbedRight)
	{
		return;
	}

	// ���� ���� ���·� ��ȯ
	bIsGrabbedRight = false;

	if (heldComponentRight)
	{
		if (heldComponentRight->TryRelease())
		{
			heldComponentRight = nullptr;
		}
	}
}

UGrabComponent* AEscapePlayer::GetGrabComponentNearMotionController(UMotionControllerComponent* motionController)
{
	// �߽���
	FVector center = motionController->GetComponentLocation();

	// �浹üũ
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(motionController);

	// �浹�� ��ü�� ����� �迭
	TArray<FOverlapResult> hitObjects;

	FCollisionObjectQueryParams objectParams;
	objectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	objectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel6);

	bool bHit = GetWorld()->OverlapMultiByObjectType(hitObjects, center, FQuat::Identity, objectParams, FCollisionShape::MakeSphere(grabRange), params);

	// �浹���� �ʾҴٸ� �ƹ�ó���� ���� �ʴ´�.
	if (!bHit)
	{
		return nullptr;
	}
	
	// ���� ����� ��ü �Ÿ�
	float closestDistance = 1e9;
	UGrabComponent* nearestGrabComponent = nullptr;
	for (int32 i = 0; i < hitObjects.Num(); i++)
	{
		TArray<UGrabComponent*> tempGrabComps;
		hitObjects[i].GetActor()->GetComponents<UGrabComponent>(tempGrabComps);
		if (tempGrabComps.Num() == 0)
		{
			continue;
		}

		if (motionController->MotionSource == FName("Right"))
		{
			bIsGrabbedRight = true;
		}
		else if (motionController->MotionSource == FName("Left"))
		{
			bIsGrabbedLeft = true;
		}

		for (auto grabComp : tempGrabComps)
		{
			// �����ϰ� �ִ� Object �Ÿ�
			float curDistance = FVector::Dist(grabComp->GetComponentLocation(), center);
			// ���� �����ϰ� �ִ� Object �Ÿ��� ���� ���� ����� Object �Ÿ����� �����ٸ�
			if (curDistance <= closestDistance)
			{
				closestDistance = curDistance;
				nearestGrabComponent = grabComp;
			}
		}
	}

	return nearestGrabComponent;
}

void AEscapePlayer::Fire()
{
	if (grabbedGun)
	{
		grabbedGun->Fire(fireAlpha);
	}
}

void AEscapePlayer::FireCompleted()
{
	if (grabbedGun)
	{
		grabbedGun->Fire(0.0f);
		grabbedGun->bFireCompleted = false;
	}
}

void AEscapePlayer::DropMagazine()
{
	if (grabbedGun)
	{
		grabbedGun->DropMagazine();
	}
}

