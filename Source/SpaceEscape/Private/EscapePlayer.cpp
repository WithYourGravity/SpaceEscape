// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapePlayer.h"
#include "Crosshair.h"
#include "DamageWidget.h"
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
#include "PlayerInfoWidget.h"
#include "RoomManager.h"
#include "SpaceEscapeGameModeBase.h"
#include "SpaceShip.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEscapePlayer::AEscapePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(78.0f);
	GetCapsuleComponent()->SetCapsuleRadius(30.0f);

	vrCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	vrCamera->SetupAttachment(RootComponent);
	vrCamera->bUsePawnControlRotation = false;
	vrCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

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

	// Watch
	watch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("watch"));
	watch->SetupAttachment(leftHand);
	watch->SetCollisionProfileName(FName("NoCollision"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempWatchMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Watch/Watch.Watch'"));
	if (tempWatchMesh.Succeeded())
	{
		watch->SetStaticMesh(tempWatchMesh.Object);
		watch->SetRelativeLocation(FVector(-3.6f, 1.0f, 5.0f));
		watch->SetRelativeRotation(FRotator(-60.0f, 45.0f, -120.0f));
		watch->SetRelativeScale3D(FVector(3.5f));
	}

	// Widget
	infoWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("infoWidgetComp"));
	infoWidgetComp->SetupAttachment(watch);
	infoWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 2.3f));
	infoWidgetComp->SetRelativeRotation(FRotator(90.0f, 0.0f, -90.0f));
	infoWidgetComp->SetRelativeScale3D(FVector(0.0035f));

	dieWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("dieWidgetComp"));
	dieWidgetComp->SetupAttachment(RootComponent);
	dieWidgetComp->SetRelativeLocation(FVector(260.0f, 0.0f, 0.0f));
	dieWidgetComp->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	dieWidgetComp->SetRelativeScale3D(FVector(0.5f));
	dieWidgetComp->SetVisibility(false);

	damageWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("damageWidgetComp"));
	damageWidgetComp->SetupAttachment(vrCamera);
	damageWidgetComp->SetVisibility(false);

	// Teleport
	teleportCircle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("teleportCircle"));
	teleportCircle->SetupAttachment(RootComponent);
	teleportCircle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	teleportCircle->SetVisibility(false);

	// Teleport Curve Niagara
	teleportCurveComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("teleportCurveComp"));
	teleportCurveComp->SetupAttachment(RootComponent);
	teleportCurveComp->SetVisibility(false);

	gunStorageComp = CreateDefaultSubobject<USceneComponent>(TEXT("gunStorageComp"));
	gunStorageComp->SetupAttachment(vrCamera);
	gunStorageComp->SetRelativeLocation(FVector(-34.0f, 0.0f, -4.0f));

	gunOverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("gunOverlapComp"));
	gunOverlapComp->SetupAttachment(gunStorageComp);
	gunOverlapComp->SetBoxExtent(FVector(35, 100, 40));
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

	// moveMode 가 TELEPORT 이면 텔레포트 기능 초기화
	if (moveMode == EMoveModeState::TELEPORT)
	{
		ResetTeleport();
	}

	currentLocation = GetActorLocation();

	HP = maxHP;

	infoUI = Cast<UPlayerInfoWidget>(infoWidgetComp->GetUserWidgetObject());
	damageUI = Cast<UDamageWidget>(damageWidgetComp->GetUserWidgetObject());

	gunOverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AEscapePlayer::OnGunStorageOverlap);
	gunOverlapComp->OnComponentEndOverlap.AddDynamic(this, &AEscapePlayer::EndGunStorageOverlap);

	roomManager = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoomManager::StaticClass()));
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
		InputSystem->BindAction(IA_Sense, ETriggerEvent::Started, this, &AEscapePlayer::CallSenseOn);
		InputSystem->BindAction(IA_Sense, ETriggerEvent::Completed, this, &AEscapePlayer::CallSenseOff);
		InputSystem->BindAction(IA_BoardShip, ETriggerEvent::Started, this, &AEscapePlayer::CallBoardingShip);
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
	if (bHit && hitInfo.GetActor()->GetActorNameOrLabel().Contains(TEXT("Floor")))
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
	FVector pos = leftAim->GetComponentLocation();
	FVector dir = leftAim->GetForwardVector() * curvedPower;

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

void AEscapePlayer::TryGrabLeft()
{
	UGrabComponent* grabComp = GetGrabComponentNearMotionController(leftHand);

	if (bIsOverlapGunStorage && storedGun)
	{
		bIsGrabbedLeft = true;
		grabComp = storedGun->grabComp;
		storedGun->SetActorHiddenInGame(false);
		storedGun->gunMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
		storedGun = nullptr;
	}

	// 만약 잡았다면
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

	if (bIsOverlapGunStorage && storedGun)
	{
		bIsGrabbedRight = true;
		grabComp = storedGun->grabComp;
		storedGun->SetActorHiddenInGame(false);
		storedGun->gunMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
		storedGun = nullptr;
	}

	// 만약 잡았다면
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

	// 잡지 않은 상태로 전환
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

	// 잡지 않은 상태로 전환
	bIsGrabbedRight = false;

	if (heldComponentRight)
	{
		if (heldComponentRight->TryRelease())
		{
			heldComponentRight = nullptr;
		}
	}
}

UGrabComponent* AEscapePlayer::

GetGrabComponentNearMotionController(UMotionControllerComponent* motionController)
{
	// 중심점
	FVector center = motionController->GetComponentLocation();

	// 충돌체크
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(motionController);

	// 충돌한 물체들 기록할 배열
	TArray<FOverlapResult> hitObjects;

	FCollisionObjectQueryParams objectParams;
	objectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	objectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel6);

	bool bHit = GetWorld()->OverlapMultiByObjectType(hitObjects, center, FQuat::Identity, objectParams, FCollisionShape::MakeSphere(grabRange), params);

	// 충돌하지 않았다면 아무처리도 하지 않는다.
	if (!bHit)
	{
		return nullptr;
	}
	
	// 가장 가까운 물체 거리
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
			// 검출하고 있는 Object 거리
			float curDistance = FVector::Dist(grabComp->GetComponentLocation(), center);
			// 만약 검출하고 있는 Object 거리가 현재 가장 가까운 Object 거리보다 가깝다면
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

void AEscapePlayer::Die()
{
	ASpaceEscapeGameModeBase* gm = Cast<ASpaceEscapeGameModeBase>(GetWorld()->GetAuthGameMode());
	gm->StopPlayTime();

	FInputModeUIOnly inputMode;
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	GetWorld()->GetFirstPlayerController()->SetInputMode(inputMode);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	dieWidgetComp->SetVisibility(true);

	FRotator endRot = GetActorRotation().Add(0.0f, 0.0f, 90.0f);
	
	GetWorld()->GetTimerManager().SetTimer(dieTimer, FTimerDelegate::CreateLambda([this, endRot]()->void
		{
			FRotator curRot = GetActorRotation();
			
			curRot =  FQuat::Slerp(curRot.Quaternion(), endRot.Quaternion(), 1.0f * GetWorld()->DeltaTimeSeconds).Rotator();

			SetActorRotation(curRot);

			if (curRot.Equals(endRot))
			{
				SetActorRotation(endRot);
				GetWorld()->GetTimerManager().ClearTimer(dieTimer);
			}
		}), 0.02f, true);
}

void AEscapePlayer::OnGunStorageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp == leftIndexFingerCollision || OtherComp == rightIndexFingerCollision)
	{
		bIsOverlapGunStorage = true;
	}
}

void AEscapePlayer::EndGunStorageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp == leftIndexFingerCollision || OtherComp == rightIndexFingerCollision)
	{
		bIsOverlapGunStorage = false;
	}
}

void AEscapePlayer::CallBoardingShip()
{
	auto ship = Cast<ASpaceShip>(UGameplayStatics::GetActorOfClass(this, ASpaceShip::StaticClass()));
	ship->BoardingShip();
}

void AEscapePlayer::CallSenseOn()
{
	roomManager->SenseOn();
}

void AEscapePlayer::CallSenseOff()
{
	roomManager->SenseOff();
}
