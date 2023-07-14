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
	class UInputAction* IA_Sense;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_BoardShip;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_WidgetLeft;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_WidgetRight;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Cheat1;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Cheat2;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Cheat3;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Cheat4;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Help;

	// �̵�ó�� �Լ�
	void Move(const FInputActionValue& values);
	// ȸ��ó�� �Լ�
	void Turn(const FInputActionValue& values);

public:
	// Camera
	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
	class UCameraComponent* vrCamera;

	UPROPERTY(VisibleAnywhere, Category = "Light")
	class USpotLightComponent* spotLight;

	// Controller
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* leftHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* rightHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* leftAim;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* rightAim;

	// WidgetInteraction
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	class UWidgetInteractionComponent* leftWidgetInteractionComp;
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	class UWidgetInteractionComponent* rightWidgetInteractionComp;

	// Hand
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController")
	class USkeletalMeshComponent* leftHandMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController")
	class USkeletalMeshComponent* rightHandMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* rightIndexFingerCollision;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* leftIndexFingerCollision;

	UPROPERTY(VisibleAnywhere, Category = "Watch")
	class UStaticMeshComponent* watch;

	// Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* infoWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UPlayerInfoWidget* infoUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* dieWidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* damageWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UDamageWidget* damageUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* helpWidgetComp;

	UPROPERTY()
	class UHelpWidget* helpWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* dialogueWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UDialogueWidget* dialogueUI;
	

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Grab")
	float grabRange = 8;
	// Grab Object ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab")
	class UGrabComponent* heldComponentRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab")
	class UGrabComponent* heldComponentLeft;

	// Object Grab ����
	UPROPERTY(BlueprintReadOnly, Category = "Grab")
	bool bIsGrabbedRight = false;
	UPROPERTY(BlueprintReadOnly, Category = "Grab")
	bool bIsGrabbedLeft = false;
	
	// ��� �õ� ���
	void TryGrabLeft();
	void TryGrabRight();
	// ����
	void UnTryGrabLeft();
	void UnTryGrabRight();

	// MotionController ��ó ���� �� �ִ� Object ã�� �Լ�
	class UGrabComponent* GetGrabComponentNearMotionController(class UMotionControllerComponent* motionController);

	UFUNCTION(BlueprintImplementableEvent)
	void R_TraceFingerData();
	UFUNCTION(BlueprintImplementableEvent)
	void L_TraceFingerData();


// Fire
private:
	void Fire();
	void FireCompleted();
	void DropMagazine();

public:
	UPROPERTY(BlueprintReadOnly)
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
	
	FTimerHandle dieTimer;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	int32 maxHP = 100;

	FORCEINLINE int32 GetHP() const { return HP; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SubtractHP(int32 damage) { HP -= damage; }

	void Die();

// Storage
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Storage")
	class USceneComponent* gunStorageComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Storage")
	class UBoxComponent* gunOverlapComp;

	UPROPERTY()
	class AGun* storedGun;

	bool bIsOverlapGunStorage = false;

	UFUNCTION()
	void OnGunStorageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndGunStorageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


// Board
	UFUNCTION()
	void CallBoardingShip();

	UPROPERTY()
	class ARoomManager* roomManager;

	void CallSenseOn();
	void CallSenseOff();

// Widget Interaction
	void SelectUIInputLeft();
	void SelectUIInputRight();
	void ReleaseUIInputLeft();
	void ReleaseUIInputRight();

	void ActiveLeftWidgetInteraction();
	void ActiveRightWidgetInteraction();
	void DeactivateLeftWidgetInteraction();
	void DeactivateRightWidgetInteraction();


// Sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* playerAttackSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* playerDieSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* playerBreathSound;

	FTimerHandle breathTimer;
	float breathMinTime = 20.0f;
	float breathMaxTime = 40.0f;

	void PlayBreathingSound();

	UFUNCTION()
	void StopBreathingSound();

	UPROPERTY()
	class ASpaceEscapeGameModeBase* gm;

	void CallCheat1();
	void CallCheat2();
	void CallCheat3();
	void CallCheat4();
	void ShowHelp();
	void HiddenHelp();

	FTimerHandle dialogueTimer;

	UFUNCTION()
	void StartDialogue();
	void ShowDialogue();
	void HiddenDialogue();

public:
	bool bIsActiveWidgetInteraction = false;

	UPROPERTY(VisibleAnywhere, Category = "WidgetInteraction")
	class UNiagaraComponent* debugLine;

	void DrawWidgetDebugLine();
};
