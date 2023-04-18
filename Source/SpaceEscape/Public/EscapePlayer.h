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
	class UInputAction* IA_GrabLeft;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_GrabRight;
	// Input Action for Fire
	//UPROPERTY(EditDefaultsOnly, Category = "Input")
	//class UInputAction* IA_Fire;

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

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* indexFingerCollision;

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
	float grabRange = 10;
	// Grab Object 기억
	UPROPERTY()
	class UGrabComponent* heldComponentRight;
	UPROPERTY()
	class UGrabComponent* heldComponentLeft;

	// Object Grab 여부
	bool bIsGrabbedRight = false;
	bool bIsGrabbedLeft = false;
	bool bIsGrabbed = false;
	
	// 잡기 시도 기능
	void TryGrabLeft();
	void TryGrabRight();
	// 놓기
	void UnTryGrabLeft();
	void UnTryGrabRight();

	// MotionController 근처 잡을 수 있는 Object 찾는 함수
	class UGrabComponent* GetGrabComponentNearMotionController(class UMotionControllerComponent* motionController);


// Fire
private:
	UPROPERTY(EditAnywhere, Category = "Fire", meta = (AllowPrivateAccess = true))
	float fireDistance = 10000.0f;

	//void Fire(const FInputActionValue& values);

};
