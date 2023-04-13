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

	// Enhanced Input 사용처리
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

	// HMD 가 연결되어 있다면
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		// 기본 tracking offset 설정
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	}
	// HMD 가 연결되어 있지 않다면
	else
	{
		// Hand 를 테스트할 수 있는 위치로 이동
		rightHand->SetRelativeLocation(FVector(20, 20, 0));
		rightAim->SetRelativeLocation(FVector(20, 20, 0));
		// 카메라의 UsePawnControlRotation 활성화
		vrCamera->bUsePawnControlRotation = true;
	}

	moveMode = EMoveModeState::JOYSTICK;

	// moveMode 가 TELEPORT 이면 텔레포트 기능 초기화
	if (moveMode == EMoveModeState::TELEPORT)
	{
		ResetTeleport();
	}
}

// Called every frame
void AEscapePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// HMD 가 연결되어 있지 않으면
	if (!UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		// 손이 카메라 방향과 일치하도록 한다.
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
	// HMD 가 연결되어 있지 않다면
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		AddMovementInput(GetActorForwardVector(), axis.X);
		AddMovementInput(GetActorRightVector(), axis.Y);
	}
	// 만약 HMD 가 연결되어 있다면
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

	// 텔레포트가 불가능하다면
	if (!ResetTeleport())
	{
		// 다음 처리를 하지 않는다.
		return;
	}

	// 텔레포트가 가능하다면 텔레포트 위치로 이동
	SetActorLocation(teleportLocation + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
}

bool AEscapePlayer::ResetTeleport()
{
	// teleportCircle 이 활성화되어 있을 때만 텔레포트 가능
	bool bCanTeleport = teleportCircle->GetVisibleFlag();

	// teleportCircle 안보이게 하기
	teleportCircle->SetVisibility(false);
	bTeleporting = false;
	teleportCurveComp->SetVisibility(false);

	return bCanTeleport;
}

bool AEscapePlayer::CheckHitTeleport(FVector lastPos, FVector& curPos)
{
	// 두 점 사이에 충돌체 있는지 확인
	FHitResult hitInfo;
	bool bHit = HitTest(lastPos, curPos, hitInfo);

	// 만약 부딪힌 대상이 바닥이라면
	if (bHit && hitInfo.GetActor()->GetName().Contains(TEXT("Floor")))
	{
		// endPos 를 부딪힌 곳으로 수정
		curPos = hitInfo.Location;
		// teleportCircle 활성화
		teleportCircle->SetVisibility(true);
		// teleportCircle 위치
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
	// 자기자신 무시
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, lastPos, curPos, ECC_Visibility, params);

	return bHit;
}

void AEscapePlayer::DrawTeleportCurve()
{
	// linePoints 초기화
	linePoints.Empty();

	// 투사체 던지기
	FVector pos = rightAim->GetComponentLocation();
	FVector dir = rightAim->GetForwardVector() * curvedPower;

	// 시작점을 가장 먼저 기록
	linePoints.Add(pos);

	for (int32 i = 0; i < lineSmooth; i++)
	{
		// 이전 점 기억
		FVector lastPos = pos;

		// 투사체 이동
		// v = v0 + at
		dir += FVector::UpVector * gravity * simulatedTime;
		// P = P0 + vt
		pos += dir * simulatedTime;

		// 점과 점 사이에 물체가 가로막고 있다면
		if (CheckHitTeleport(lastPos, pos))
		{
			// 그 점을 마지막 점으로 한다.
			linePoints.Add(pos);
			break;
		}

		// 투사체 위치 기록
		linePoints.Add(pos);
	}
}

void AEscapePlayer::TryGrab()
{
	// Remote Grab 활성화되어 있으면
	if (bIsRemoteGrab)
	{
		RemoteGrab();
		return;
	}

	// 중심점
	FVector center = rightHand->GetComponentLocation();

	// 충돌체크
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(rightHand);

	// 충돌한 물체들 기록할 배열
	TArray<FOverlapResult> hitObjects;
	bool bHit = GetWorld()->OverlapMultiByChannel(hitObjects, center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(grabRange), params);

	// 충돌하지 않았다면 아무처리도 하지 않는다.
	if (!bHit)
	{
		return;
	}

	// 가장 가까운 물체 인덱스
	int32 closestIdx = 0;
	for (int32 i = 0; i < hitObjects.Num(); i++)
	{
		// 물리 기능이 활성화되어 있는 object 만 판단
		// 만약 부딪힌 컴포넌트가 물리기능이 비활성화되어 있다면
		if (!hitObjects[i].GetComponent()->IsSimulatingPhysics())
		{
			// 검출하지 않는다.
			continue;
		}

		bIsGrabbed = true;

		// 현재 가장 가까운 Object 거리
		float closestDist = FVector::Dist(hitObjects[closestIdx].GetActor()->GetActorLocation(), center);
		// 검출하고 있는 Object 거리
		float nextDist = FVector::Dist(hitObjects[i].GetActor()->GetActorLocation(), center);
		// 만약 검출하고 있는 Object 거리가 현재 가장 가까운 Object 거리보다 가깝다면
		if (nextDist < closestDist)
		{
			closestIdx = i;
		}
	}

	// 만약 잡았다면
	if (bIsGrabbed)
	{
		grabbedObject = hitObjects[closestIdx].GetComponent();
		// Object 물리기능 비활성화
		grabbedObject->SetSimulatePhysics(false);
		grabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 손에 붙여준다.
		grabbedObject->AttachToComponent(rightHand, FAttachmentTransformRules::KeepWorldTransform);

		// 직전 위치 초기화
		prevPos = rightHand->GetComponentLocation();
		// 직전 회전값 초기화
		prevRot = rightHand->GetComponentQuat();
	}
}

void AEscapePlayer::UnTryGrab()
{
	if (!bIsGrabbed)
	{
		return;
	}

	// 잡지 않은 상태로 전환
	bIsGrabbed = false;
	// 손에서 떼어내기
	grabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 물리기능 활성화
	grabbedObject->SetSimulatePhysics(true);
	// 충돌기능 활성화
	grabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 던지기
	grabbedObject->AddForce(throwDirection * throwPower * grabbedObject->GetMass());

	// 회전 시키기
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

	// 던질 방향 업데이트
	throwDirection = rightHand->GetComponentLocation() - prevPos;

	// 이전 위치 업데이트
	prevPos = rightHand->GetComponentLocation();

	// 회전 방향 업데이트
	deltaRotation = rightHand->GetComponentQuat() * prevRot.Inverse();

	// 이전 회전값 업데이트
	prevRot = rightHand->GetComponentQuat();
}

void AEscapePlayer::RemoteGrab()
{
	// 충돌 체크
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(rightAim);

	FVector startPos = rightAim->GetComponentLocation();
	FVector endPos = startPos + rightAim->GetForwardVector() * remoteDistance;

	FHitResult hitInfo;
	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startPos, endPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(remoteRadius), params);

	// 충돌이 되면 잡아당기기 애니메이션 실행
	if (bHit && hitInfo.GetComponent()->IsSimulatingPhysics())
	{
		bIsGrabbed = true;

		// 잡은 물체 할당
		grabbedObject = hitInfo.GetComponent();
		// Object 물리기능 비활성화
		grabbedObject->SetSimulatePhysics(false);
		grabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 손에 붙여준다.
		grabbedObject->AttachToComponent(rightHand, FAttachmentTransformRules::KeepWorldTransform);

		// 원거리 물체가 손으로 끌려오도록 처리
		GetWorld()->GetTimerManager().SetTimer(grabTimer, FTimerDelegate::CreateLambda(
		[this]()->void
					{
						// 이동 중간에 사용자가 놓아버리면
						if (grabbedObject == nullptr)
						{
							GetWorld()->GetTimerManager().ClearTimer(grabTimer);
							return;
						}

						// 물체 -> 손
						FVector pos = grabbedObject->GetComponentLocation();
						FVector targetPos = rightHand->GetComponentLocation();
						pos = FMath::Lerp<FVector>(pos, targetPos, remoteMoveSpeed * GetWorld()->DeltaTimeSeconds);
						grabbedObject->SetWorldLocation(pos);

						float distance = FVector::Dist(pos, targetPos);
						// 거의 가까워졌다면
						if (distance < 10)
						{
							// 이동 중단하기
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
	// 시각화 여부 확인, Remote Grab 활성화 여부
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

	// 그리기
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

	// 만약 부딪힌 것이 있으면
	if (bHit)
	{
		DrawDebugSphere(GetWorld(), hitInfo.Location, remoteRadius, 10, FColor::Blue);

		auto hitComp = hitInfo.GetComponent();
		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			// 날려보낸다.
			hitComp->AddForceAtLocation((endPos - startPos).GetSafeNormal() * 150000, hitInfo.Location);
		}
	}
}
