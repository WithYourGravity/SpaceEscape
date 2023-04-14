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

	// �̵�ó�� �Լ�
	void Move(const FInputActionValue& values);
	// ȸ��ó�� �Լ�
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

	// �ڷ���Ʈ ���� �Լ�
	void TeleportStart(const FInputActionValue& values);
	// �ڷ���Ʈ �� �Լ�
	void TeleportEnd(const FInputActionValue& values);
	// �ڷ���Ʈ ��� ����
	bool ResetTeleport();
	// �ڷ���Ʈ �� �浹 ó�� �Լ�
	bool CheckHitTeleport(FVector lastPos, FVector& curPos);

	// �浹 ó�� �Լ�
	bool HitTest(FVector lastPos, FVector curPos, FHitResult& hitInfo);

	// �ڷ���Ʈ�� ��ġ
	FVector teleportLocation;

	// �̵� ���
	UPROPERTY(EditAnywhere, Category = "Settings")
	EMoveModeState moveMode = EMoveModeState::TELEPORT;;

private:
	// Curve Teleport
	// �ڷ���Ʈ ��� Ȱ��ȭ ����
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	bool bTeleporting = false;
	// ������ ��
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	float curvedPower = 1500.0f;
	// �߷�
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	float gravity = -5000.0f;
	// �ùķ��̼� �ð�
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	float simulatedTime = 0.02f;
	// ��� �̷�� �� ����
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	int32 lineSmooth = 40;
	// ���� ����� �迭
	UPROPERTY()
	TArray<FVector> linePoints;

	// �ڷ���Ʈ ó��
	void DrawTeleportCurve();


// Grab
public:
	// Grab ����
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float grabRange = 100;
	// Grab Object ���
	UPROPERTY()
	class UPrimitiveComponent* grabbedObject;

	// Object Grab ����
	bool bIsGrabbed = false;

	// ���� ��
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float throwPower = 10000.0f;
	// ���� ����
	FVector throwDirection;
	// ���� ��ġ
	FVector prevPos;
	// ȸ�� ����
	FQuat deltaRotation;
	// ���� ȸ����
	FQuat prevRot;
	// ȸ�� ������
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float torquePower = 1.0f;

	// ������� ��� ����
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	bool bIsRemoteGrab = false;
	// Remote Grab �Ÿ�
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float remoteDistance = 2000.0f;
	// �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float remoteMoveSpeed = 10.0f;
	// Remote Grab ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float remoteRadius = 20.0f;
	// Remote Object �̵��� ���� Ÿ�̸�
	FTimerHandle grabTimer;

	// Remote Grab �ð�ȭ ����
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	bool bDrawDebugRemoteGrab = false;

	// ��� �õ� ���
	void TryGrab();
	// ����
	void UnTryGrab();
	// ����ִ� ��, ���� ������ ������Ʈ�ϱ� ���� �Լ�
	void Grabbing();

	// ���� ��� �Լ�
	void RemoteGrab();
	// ���� ��� �ð�ȭ �Լ�
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
