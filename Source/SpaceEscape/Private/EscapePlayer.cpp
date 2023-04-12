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
	vrCamera->bUsePawnControlRotation = true;

	// MotionController
	leftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	leftHand->SetupAttachment(RootComponent);
	leftHand->SetTrackingMotionSource(FName("Left"));

	rightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	rightHand->SetupAttachment(RootComponent);
	rightHand->SetTrackingMotionSource(FName("Right"));

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
		}
	}

	moveMode = EMoveModeState::TELEPORT;
	
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
	}
	
}

void AEscapePlayer::Move(const FInputActionValue& values)
{
	if (moveMode == EMoveModeState::TELEPORT)
	{
		return;
	}

	FVector2D axis = values.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), axis.X);
	AddMovementInput(GetActorRightVector(), axis.Y);
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
	FVector pos = rightHand->GetComponentLocation();
	FVector dir = rightHand->GetForwardVector() * throwPower;

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
