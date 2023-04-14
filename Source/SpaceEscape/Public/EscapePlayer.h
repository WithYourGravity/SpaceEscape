// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EscapePlayer.generated.h"

UENUM(BlueprintType)
enum class EMoveModeState : uint8
{
	JOYSTICK,
	TELEPORT,
};

UCLASS()
class SPACEESCAPE_API AEscapePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEscapePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_PlayerInput;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Hand;

	// Input Action for Move
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;
	// Input Action for Mouse
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Mouse;
	// Input Action for Teleport
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Teleport;
	// Input Action for Grab
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Grab;
	// Input Action for Fire
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;

	// 이동처리 함수
	void Move(const FInputActionValue& values);
	// 회전처리 함수
	void Turn(const FInputActionValue& values);

public:
	// Camera
	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
	class UCameraComponent* vrCamera;

	// Controller
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* leftHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* rightHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* leftAim;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* rightAim;

	// Hand
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController")
	class USkeletalMeshComponent* leftHandMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController")
	class USkeletalMeshComponent* rightHandMesh;


// Teleport
public:
	// Teleport Circle Niagara
	UPROPERTY(VisibleAnywhere, Category = "Teleport")
	class UNiagaraComponent* teleportCircle;
	// Teleport Curve Niagara
	UPROPERTY(VisibleAnywhere, Category = "Teleport")
	class UNiagaraComponent* teleportCurveComp;

	// 텔레포트 시작 함수
	void TeleportStart(const FInputActionValue& values);
	// 텔레포트 끝 함수
	void TeleportEnd(const FInputActionValue& values);
	// 텔레포트 기능 리셋
	bool ResetTeleport();
	// 텔레포트 선 충돌 처리 함수
	bool CheckHitTeleport(FVector lastPos, FVector& curPos);

	// 충돌 처리 함수
	bool HitTest(FVector lastPos, FVector curPos, FHitResult& hitInfo);

	// 텔레포트할 위치
	FVector teleportLocation;

	// 이동 방식
	UPROPERTY(EditAnywhere, Category = "Settings")
	EMoveModeState moveMode = EMoveModeState::TELEPORT;;

private:
	// Curve Teleport
	// 텔레포트 기능 활성화 여부
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	bool bTeleporting = false;
	// 던지는 힘
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	float curvedPower = 1500.0f;
	// 중력
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	float gravity = -5000.0f;
	// 시뮬레이션 시간
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	float simulatedTime = 0.02f;
	// 곡선을 이루는 점 개수
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	int32 lineSmooth = 40;
	// 점을 기억할 배열
	UPROPERTY()
	TArray<FVector> linePoints;

	// 텔레포트 처리
	void DrawTeleportCurve();


// Grab
public:
	// Grab 범위
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float grabRange = 100;
	// Grab Object 기억
	UPROPERTY()
	class UPrimitiveComponent* grabbedObject;

	// Object Grab 여부
	bool bIsGrabbed = false;

	// 던질 힘
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float throwPower = 10000.0f;
	// 던질 방향
	FVector throwDirection;
	// 직전 위치
	FVector prevPos;
	// 회전 방향
	FQuat deltaRotation;
	// 이전 회전값
	FQuat prevRot;
	// 회전 빠르기
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float torquePower = 1.0f;

	// 원격잡기 모드 여부
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	bool bIsRemoteGrab = false;
	// Remote Grab 거리
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float remoteDistance = 2000.0f;
	// 이동 속도
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float remoteMoveSpeed = 10.0f;
	// Remote Grab 검출 범위
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float remoteRadius = 20.0f;
	// Remote Object 이동을 위한 타이머
	FTimerHandle grabTimer;

	// Remote Grab 시각화 여부
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	bool bDrawDebugRemoteGrab = false;

	// 잡기 시도 기능
	void TryGrab();
	// 놓기
	void UnTryGrab();
	// 잡고있는 중, 던질 정보를 업데이트하기 위한 함수
	void Grabbing();

	// 원격 잡기 함수
	void RemoteGrab();
	// 원격 잡기 시각화 함수
	void DrawDebugRemoteGrab();


// Fire
private:
	UPROPERTY(EditAnywhere, Category = "Fire", meta = (AllowPrivateAccess = true))
	float fireDistance = 10000.0f;

	void Fire(const FInputActionValue& values);

	// Crosshair
	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ACrosshair> crosshairFactory;

	// Crosshair Instance
	UPROPERTY()
	class ACrosshair* crosshair;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	float crosshairScale = 0.3f;

	// Draw Crosshair
	void DrawCrosshair();
};
