// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeJoystick.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "MotionControllerComponent.h"
#include "PuzzleRoomThreePathFinding.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APuzzleRoomThreeJoystick::APuzzleRoomThreeJoystick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);
	sceneComp->SetRelativeScale3D(FVector(0.43f));

	baseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("baseMeshComp"));
	baseMeshComp->SetupAttachment(RootComponent);
	baseMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempBaseMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_JoystickBase.SM_JoystickBase'"));
    if (tempBaseMesh.Succeeded())
    {
		baseMeshComp->SetStaticMesh(tempBaseMesh.Object);
    }

	stickMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("stickMeshComp"));
	stickMeshComp->SetupAttachment(RootComponent);
	stickMeshComp->SetRelativeLocation(FVector(0, 0, 15.f));
	stickMeshComp->SetRelativeRotation(FRotator(0, 90.f, 0));
	stickMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	stickMeshComp->SetSimulatePhysics(true);
	stickMeshComp->SetRelativeScale3D(FVector(3.f));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempStickMesh(TEXT("/Script/Engine.StaticMesh'/Game/Yeni/Assets/MainSpaceShip/Meshes/PirateFighter/SM_PirateFighter_Joystick.SM_PirateFighter_Joystick'"));
	if (tempStickMesh.Succeeded())
	{
		stickMeshComp->SetStaticMesh(tempStickMesh.Object);
	}
	
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(stickMeshComp);
	grabComp->grabType = EGrabType::LEVER;
	grabComp->SetRelativeLocation(FVector(1.f, 0, 7.f));
	grabComp->SetRelativeRotation(FRotator(90.f, 0, 0));
	
	constraintComp = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("constraintComp"));
	constraintComp->SetupAttachment(RootComponent);
	constraintComp->SetRelativeRotation(FRotator(90.f, 0, 0));
	constraintComp->SetDisableCollision(true);
	constraintComp->SetAngularSwing1Limit(ACM_Limited, 45.f);
	constraintComp->SetAngularSwing2Limit(ACM_Limited, 45.f);
	constraintComp->SetAngularTwistLimit(ACM_Locked, 0);
	constraintComp->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	constraintComp->SetOrientationDriveTwistAndSwing(false, true);
	constraintComp->SetAngularVelocityDriveTwistAndSwing(false, true);
	constraintComp->SetAngularDriveParams(2000, 20, 0);

	stickPosComp = CreateDefaultSubobject<USphereComponent>(TEXT("stickPosComp"));
	stickPosComp->SetupAttachment(stickMeshComp);
	stickPosComp->SetRelativeLocation(FVector(0, 0, 12.f));
	stickPosComp->SetSphereRadius(2.f);
	stickPosComp->SetCollisionProfileName(FName("FingerPreset"));

	forwardPosComp = CreateDefaultSubobject<USphereComponent>(TEXT("forwardPosComp"));
	forwardPosComp->SetupAttachment(RootComponent);
	forwardPosComp->SetSphereRadius(9.f);
	forwardPosComp->SetRelativeLocation(FVector(0, -20.f, 45.f));
	forwardPosComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	backPosComp = CreateDefaultSubobject<USphereComponent>(TEXT("backPosComp"));
	backPosComp->SetupAttachment(RootComponent);
	backPosComp->SetSphereRadius(9.f);
	backPosComp->SetRelativeLocation(FVector(0, 20.f, 45.f));
	backPosComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	leftPosComp = CreateDefaultSubobject<USphereComponent>(TEXT("leftPosComp"));
	leftPosComp->SetupAttachment(RootComponent);
	leftPosComp->SetSphereRadius(9.f);
	leftPosComp->SetRelativeLocation(FVector(-20.f, 0, 45.f));
	leftPosComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	rightPosComp = CreateDefaultSubobject<USphereComponent>(TEXT("rightPosComp"));
	rightPosComp->SetupAttachment(RootComponent);
	rightPosComp->SetSphereRadius(9.f);
	rightPosComp->SetRelativeLocation(FVector(20.f, 0, 45.f));
	rightPosComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	resetButtonComp = CreateDefaultSubobject<USphereComponent>(TEXT("resetButtonComp"));
	resetButtonComp->SetupAttachment(RootComponent);
	resetButtonComp->SetSphereRadius(6.f);
	resetButtonComp->SetRelativeLocation(FVector(12.f, 10.f, 16.f));
	resetButtonComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base>tempHaptic(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/LTG/UI/HF_TouchFeedback.HF_TouchFeedback'"));
	if (tempHaptic.Succeeded())
	{
		hapticFeedback = tempHaptic.Object;
	}

	Tags.Add(FName("Sense"));
	stickMeshComp->ComponentTags.Add(FName("Sense.R3"));

	ConstructorHelpers::FObjectFinder<USoundBase>tempSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/pathReset.pathReset'"));
    if (tempSound.Succeeded())
    {
		resetButtonSound = tempSound.Object;
    }
}

// Called when the game starts or when spawned
void APuzzleRoomThreeJoystick::BeginPlay()
{
	Super::BeginPlay();

	stickMeshComp->SetMassOverrideInKg(FName(NAME_None), 10, true);

	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	puzzlePathFinding = Cast<APuzzleRoomThreePathFinding>(UGameplayStatics::GetActorOfClass(this, APuzzleRoomThreePathFinding::StaticClass()));
	constraintComp->SetConstrainedComponents(baseMeshComp, FName("None"), stickMeshComp, FName("None"));

	grabComp->onGrabbedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));
	grabComp->onDroppedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));

	stickPosComp->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomThreeJoystick::StickOnOverlap);
	stickPosComp->OnComponentEndOverlap.AddDynamic(this, &APuzzleRoomThreeJoystick::StickEndOverlap);
	resetButtonComp->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomThreeJoystick::ResetButtonOnOverlap);

	puzzlePathFinding->puzzleClearDele.AddUFunction(this, FName("BreakConstraintWhenClear"));
}

// Called every frame
void APuzzleRoomThreeJoystick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGrabed && !puzzlePathFinding->GetbWasReport())
	{
		ControlByPlayerHand();
	}
}

void APuzzleRoomThreeJoystick::ChangeIsGrabed()
{
	bIsGrabed = !bIsGrabed;
}

void APuzzleRoomThreeJoystick::StickOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetOwner() != this || puzzlePathFinding->GetbWasReport())
	{
		return;
	}

	if (player->heldComponentLeft == this->grabComp)
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
	}
	else
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
	}

	otherCompNameForTimer = OtherComp->GetName()[0];
	GetWorldTimerManager().SetTimer(stickHandle, FTimerDelegate::CreateUObject(this, &APuzzleRoomThreeJoystick::MoveFunction, otherCompNameForTimer), 0.6f, true, 0);
}

void APuzzleRoomThreeJoystick::StickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetOwner() != this)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(stickHandle);
}

void APuzzleRoomThreeJoystick::ResetButtonOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (puzzlePathFinding->GetbWasReport() || bForSafeReset)
	{
		return;
	}

	if (OtherComp->GetName().Contains("left"))
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
	}
	else
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
	}

	// 튀어나온 상태이므로 평평한 상태로 돌린 후 초기화
	if (!puzzlePathFinding->GetbisMoving())
	{
		puzzlePathFinding->MovingTrigger();
		FTimerHandle resetHandle;
		GetWorldTimerManager().SetTimer(resetHandle, puzzlePathFinding, &APuzzleRoomThreePathFinding::ResetThisPuzzle, 2, false);

		// 리셋버튼 소리 재생
		UGameplayStatics::PlaySound2D(this, resetButtonSound);
	}

	// 한번 리셋하면 4초간 안되게끔
	bForSafeReset = true;
	FTimerHandle safeResetHandle;
	GetWorldTimerManager().SetTimer(safeResetHandle, FTimerDelegate::CreateLambda([&]()
		{
			bForSafeReset = false;
		}), 4.f, false);
}

void APuzzleRoomThreeJoystick::ControlByPlayerHand()
{
	if (player->heldComponentLeft == this->grabComp)
	{
		playerHand = player->leftHand;
	}
	else
	{
		playerHand = player->rightHand;
	}
	
	stickMeshComp->AddForceAtLocation((playerHand->GetComponentLocation() - grabComp->GetComponentLocation()) * 6000, grabComp->GetComponentLocation());
	playerHand->SetWorldLocation(grabComp->GetComponentLocation());
}

void APuzzleRoomThreeJoystick::MoveFunction(char componentName)
{
	if (componentName == 'f')
	{
		puzzlePathFinding->MovePlayingNode(EMoveDir::Forward);
	}
	else if (componentName == 'b')
	{
		puzzlePathFinding->MovePlayingNode(EMoveDir::Back);
	}
	else if (componentName == 'l')
	{
		puzzlePathFinding->MovePlayingNode(EMoveDir::Left);
	}
	else if (componentName == 'r')
	{
		puzzlePathFinding->MovePlayingNode(EMoveDir::Right);
	}
}

void APuzzleRoomThreeJoystick::BreakConstraintWhenClear()
{
	constraintComp->BreakConstraint();
	stickMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	grabComp->grabType = EGrabType::SNAP;
	grabComp->TryGrab(playerHand);
}

