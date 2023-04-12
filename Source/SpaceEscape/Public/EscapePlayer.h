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
	// Input Action for Move
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;
	// Input Action for Mouse
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Mouse;
	// Input Action for Teleport
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Teleport;

	// �̵�ó�� �Լ�
	void Move(const FInputActionValue& values);
	// ȸ��ó�� �Լ�
	void Turn(const FInputActionValue& values);

protected:
	// Camera
	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
	class UCameraComponent* vrCamera;

	// Controller
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* leftHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* rightHand;
	// Hand
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController")
	class USkeletalMeshComponent* leftHandMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController")
	class USkeletalMeshComponent* rightHandMesh;

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

	// �ڷ���Ʈ�� ��ġ
	FVector teleportLocation;

	// �浹 ó�� �Լ�
	bool HitTest(FVector lastPos, FVector curPos, FHitResult& hitInfo);

	// �̵� ���
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	EMoveModeState moveMode;

private:
	// Curve Teleport
	// �ڷ���Ʈ ��� Ȱ��ȭ ����
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	bool bTeleporting = false;
	// ������ ��
	UPROPERTY(EditAnywhere, Category = "Teleport", meta = (AllowPrivateAccess = true))
	float throwPower = 1500.0f;
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
};
