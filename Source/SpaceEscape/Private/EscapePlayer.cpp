// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapePlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

// Sets default values
AEscapePlayer::AEscapePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	vrCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	vrCamera->SetupAttachment(RootComponent);
	vrCamera->bUsePawnControlRotation = false;

	// MotionController
	leftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	leftHand->SetupAttachment(RootComponent);
	leftHand->SetTrackingMotionSource(FName("Left"));

	rightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	rightHand->SetupAttachment(RootComponent);
	rightHand->SetTrackingMotionSource(FName("Right"));

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

	moveMode = EMoveModeState::JOYSTICK;

	// moveMode �� TELEPORT �̸� �ڷ���Ʈ ��� �ʱ�ȭ
	if (moveMode == EMoveModeState::TELEPORT)
	{
		ResetTeleport();
	}
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

	Grabbing();

	DrawDebugRemoteGrab();
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
		InputSystem->BindAction(IA_Grab, ETriggerEvent::Started, this, &AEscapePlayer::TryGrab);
		InputSystem->BindAction(IA_Grab, ETriggerEvent::Completed, this, &AEscapePlayer::UnTryGrab);
		InputSystem->BindAction(IA_Fire, ETriggerEvent::Started, this, &AEscapePlayer::Fire);
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
	if (bHit && hitInfo.GetActor()->GetName().Contains(TEXT("Floor")))
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
	FVector pos = rightAim->GetComponentLocation();
	FVector dir = rightAim->GetForwardVector() * curvedPower;

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

void AEscapePlayer::TryGrab()
{
	// Remote Grab Ȱ��ȭ�Ǿ� ������
	if (bIsRemoteGrab)
	{
		RemoteGrab();
		return;
	}

	// �߽���
	FVector center = rightHand->GetComponentLocation();

	// �浹üũ
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(rightHand);

	// �浹�� ��ü�� ����� �迭
	TArray<FOverlapResult> hitObjects;
	bool bHit = GetWorld()->OverlapMultiByChannel(hitObjects, center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(grabRange), params);

	// �浹���� �ʾҴٸ� �ƹ�ó���� ���� �ʴ´�.
	if (!bHit)
	{
		return;
	}

	// ���� ����� ��ü �ε���
	int32 closestIdx = 0;
	for (int32 i = 0; i < hitObjects.Num(); i++)
	{
		// ���� ����� Ȱ��ȭ�Ǿ� �ִ� object �� �Ǵ�
		// ���� �ε��� ������Ʈ�� ��������� ��Ȱ��ȭ�Ǿ� �ִٸ�
		if (!hitObjects[i].GetComponent()->IsSimulatingPhysics())
		{
			// �������� �ʴ´�.
			continue;
		}

		bIsGrabbed = true;

		// ���� ���� ����� Object �Ÿ�
		float closestDist = FVector::Dist(hitObjects[closestIdx].GetActor()->GetActorLocation(), center);
		// �����ϰ� �ִ� Object �Ÿ�
		float nextDist = FVector::Dist(hitObjects[i].GetActor()->GetActorLocation(), center);
		// ���� �����ϰ� �ִ� Object �Ÿ��� ���� ���� ����� Object �Ÿ����� �����ٸ�
		if (nextDist < closestDist)
		{
			closestIdx = i;
		}
	}

	// ���� ��Ҵٸ�
	if (bIsGrabbed)
	{
		grabbedObject = hitObjects[closestIdx].GetComponent();
		// Object ������� ��Ȱ��ȭ
		grabbedObject->SetSimulatePhysics(false);
		grabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// �տ� �ٿ��ش�.
		grabbedObject->AttachToComponent(rightHand, FAttachmentTransformRules::KeepWorldTransform);

		// ���� ��ġ �ʱ�ȭ
		prevPos = rightHand->GetComponentLocation();
		// ���� ȸ���� �ʱ�ȭ
		prevRot = rightHand->GetComponentQuat();
	}
}

void AEscapePlayer::UnTryGrab()
{
	if (!bIsGrabbed)
	{
		return;
	}

	// ���� ���� ���·� ��ȯ
	bIsGrabbed = false;
	// �տ��� �����
	grabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// ������� Ȱ��ȭ
	grabbedObject->SetSimulatePhysics(true);
	// �浹��� Ȱ��ȭ
	grabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// ������
	grabbedObject->AddForce(throwDirection * throwPower * grabbedObject->GetMass());

	// ȸ�� ��Ű��
	float angle;
	FVector axis;
	deltaRotation.ToAxisAndAngle(axis, angle);
	float dt = GetWorld()->DeltaTimeSeconds;
	FVector angularVelocity = (1.0f / dt) * angle * axis;
	grabbedObject->SetPhysicsAngularVelocityInRadians(angularVelocity * torquePower, true);

	grabbedObject = nullptr;
}

void AEscapePlayer::Grabbing()
{
	if (!bIsGrabbed)
	{
		return;
	}

	// ���� ���� ������Ʈ
	throwDirection = rightHand->GetComponentLocation() - prevPos;

	// ���� ��ġ ������Ʈ
	prevPos = rightHand->GetComponentLocation();

	// ȸ�� ���� ������Ʈ
	deltaRotation = rightHand->GetComponentQuat() * prevRot.Inverse();

	// ���� ȸ���� ������Ʈ
	prevRot = rightHand->GetComponentQuat();
}

void AEscapePlayer::RemoteGrab()
{
	// �浹 üũ
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(rightAim);

	FVector startPos = rightAim->GetComponentLocation();
	FVector endPos = startPos + rightAim->GetForwardVector() * remoteDistance;

	FHitResult hitInfo;
	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startPos, endPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(remoteRadius), params);

	// �浹�� �Ǹ� ��ƴ��� �ִϸ��̼� ����
	if (bHit && hitInfo.GetComponent()->IsSimulatingPhysics())
	{
		bIsGrabbed = true;

		// ���� ��ü �Ҵ�
		grabbedObject = hitInfo.GetComponent();
		// Object ������� ��Ȱ��ȭ
		grabbedObject->SetSimulatePhysics(false);
		grabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// �տ� �ٿ��ش�.
		grabbedObject->AttachToComponent(rightHand, FAttachmentTransformRules::KeepWorldTransform);

		// ���Ÿ� ��ü�� ������ ���������� ó��
		GetWorld()->GetTimerManager().SetTimer(grabTimer, FTimerDelegate::CreateLambda(
		[this]()->void
					{
						// �̵� �߰��� ����ڰ� ���ƹ�����
						if (grabbedObject == nullptr)
						{
							GetWorld()->GetTimerManager().ClearTimer(grabTimer);
							return;
						}

						// ��ü -> ��
						FVector pos = grabbedObject->GetComponentLocation();
						FVector targetPos = rightHand->GetComponentLocation();
						pos = FMath::Lerp<FVector>(pos, targetPos, remoteMoveSpeed * GetWorld()->DeltaTimeSeconds);
						grabbedObject->SetWorldLocation(pos);

						float distance = FVector::Dist(pos, targetPos);
						// ���� ��������ٸ�
						if (distance < 10)
						{
							// �̵� �ߴ��ϱ�
							grabbedObject->SetWorldLocation(targetPos);

							prevPos = rightHand->GetComponentLocation();
							prevRot = rightHand->GetComponentQuat();

							GetWorld()->GetTimerManager().ClearTimer(grabTimer);
						}
					}
		), 0.02f, true);
	}
}

void AEscapePlayer::DrawDebugRemoteGrab()
{
	// �ð�ȭ ���� Ȯ��, Remote Grab Ȱ��ȭ ����
	if (!bDrawDebugRemoteGrab || !bIsRemoteGrab)
	{
		return;
	}

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(rightAim);

	FVector startPos = rightAim->GetComponentLocation();
	FVector endPos = startPos + rightAim->GetForwardVector() * remoteDistance;

	FHitResult hitInfo;
	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startPos, endPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(remoteRadius), params);

	// �׸���
	if (bHit && hitInfo.GetComponent()->IsSimulatingPhysics())
	{
		DrawDebugSphere(GetWorld(), hitInfo.Location, remoteRadius, 10, FColor::Blue);
	}
}

void AEscapePlayer::Fire(const FInputActionValue& values)
{
	FVector startPos = rightAim->GetComponentLocation();
	FVector endPos = startPos + rightAim->GetForwardVector() * 10000;

	FHitResult hitInfo;
	bool bHit = HitTest(startPos, endPos, hitInfo);

	// ���� �ε��� ���� ������
	if (bHit)
	{
		DrawDebugSphere(GetWorld(), hitInfo.Location, remoteRadius, 10, FColor::Blue);

		auto hitComp = hitInfo.GetComponent();
		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			// ����������.
			hitComp->AddForceAtLocation((endPos - startPos).GetSafeNormal() * 150000, hitInfo.Location);
		}
	}
}
