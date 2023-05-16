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
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_FireLeft;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_FireRight;
	// Input Action for Magazine
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_DropMagazineLeft;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_DropMagazineRight;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_AppearInfo;


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

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* rightIndexFingerCollision;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* leftIndexFingerCollision;

	// Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* infoWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UPlayerInfoWidget* infoUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* dieWidgetComp;

	void AppearInfoWidget();
	void DisappearInfoWidget();

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
	float grabRange = 4;
	// Grab Object ���
	UPROPERTY()
	class UGrabComponent* heldComponentRight;
	UPROPERTY()
	class UGrabComponent* heldComponentLeft;

	// Object Grab ����
	bool bIsGrabbedRight = false;
	bool bIsGrabbedLeft = false;
	bool bIsGrabbed = false;
	
	// ��� �õ� ���
	void TryGrabLeft();
	void TryGrabRight();
	// ����
	void UnTryGrabLeft();
	void UnTryGrabRight();

	// MotionController ��ó ���� �� �ִ� Object ã�� �Լ�
	class UGrabComponent* GetGrabComponentNearMotionController(class UMotionControllerComponent* motionController);


// Fire
private:
	void Fire();
	void FireCompleted();
	void DropMagazine();

public:
	UPROPERTY()
	class AGun* grabbedGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float fireAlpha = 0.0f;


// Climbing
public:
	bool bIsClimbing = false;

	FVector currentLocation;


// HP	
private:
	int32 HP;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	int32 maxHP = 30;

	FORCEINLINE int32 GetHP() const { return HP; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SubtractHP(int32 damage) { HP -= damage; }

	void Die();
};
