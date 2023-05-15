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
	sceneComp->SetRelativeScale3D(FVector(0.5f));

	baseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("baseMeshComp"));
	baseMeshComp->SetupAttachment(RootComponent);
	baseMeshComp->SetCollisionProfileName(FName("BlockAllDynamic"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempBaseMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_JoystickBase.SM_JoystickBase'"));
    if (tempBaseMesh.Succeeded())
    {
		baseMeshComp->SetStaticMesh(tempBaseMesh.Object);
    }

	stickMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("stickMeshComp"));
	stickMeshComp->SetupAttachment(RootComponent);
	stickMeshComp->SetRelativeLocation(FVector(0, 0, 15.f));
	stickMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	stickMeshComp->SetSimulatePhysics(true);
	stickMeshComp->SetMassOverrideInKg(FName(NAME_None), 10, true);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempStickMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_Joystick.SM_Joystick'"));
	if (tempStickMesh.Succeeded())
	{
		stickMeshComp->SetStaticMesh(tempStickMesh.Object);
	}
	
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(stickMeshComp);
	grabComp->grabType = EGrabType::LEVER;
	grabComp->SetRelativeLocation(FVector(0, 0, 30.f));
	
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
	stickPosComp->SetRelativeLocation(FVector(0, 0, 36.f));
	stickPosComp->SetSphereRadius(6.f);
	stickPosComp->SetCollisionProfileName(FName("FingerPreset"));

	forwardPosComp = CreateDefaultSubobject<USphereComponent>(TEXT("forwardPosComp"));
	forwardPosComp->SetupAttachment(RootComponent);
	forwardPosComp->SetSphereRadius(6.f);
	forwardPosComp->SetRelativeLocation(FVector(20.f, 0, 50.f));
	forwardPosComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	backPosComp = CreateDefaultSubobject<USphereComponent>(TEXT("backPosComp"));
	backPosComp->SetupAttachment(RootComponent);
	backPosComp->SetSphereRadius(6.f);
	backPosComp->SetRelativeLocation(FVector(-20.f, 0, 50.f));
	backPosComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	leftPosComp = CreateDefaultSubobject<USphereComponent>(TEXT("leftPosComp"));
	leftPosComp->SetupAttachment(RootComponent);
	leftPosComp->SetSphereRadius(6.f);
	leftPosComp->SetRelativeLocation(FVector(0, -20.f, 50.f));
	leftPosComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	rightPosComp = CreateDefaultSubobject<USphereComponent>(TEXT("rightPosComp"));
	rightPosComp->SetupAttachment(RootComponent);
	rightPosComp->SetSphereRadius(6.f);
	rightPosComp->SetRelativeLocation(FVector(0, 20.f, 50.f));
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
}

// Called when the game starts or when spawned
void APuzzleRoomThreeJoystick::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	puzzlePathFinding = Cast<APuzzleRoomThreePathFinding>(UGameplayStatics::GetActorOfClass(this, APuzzleRoomThreePathFinding::StaticClass()));
	constraintComp->SetConstrainedComponents(baseMeshComp, FName("None"), stickMeshComp, FName("None"));

	grabComp->onGrabbedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));
	grabComp->onDroppedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));

	stickPosComp->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomThreeJoystick::StickOnOverlap);
	stickPosComp->OnComponentEndOverlap.AddDynamic(this, &APuzzleRoomThreeJoystick::StickEndOverlap);
	resetButtonComp->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomThreeJoystick::ResetButtonOnOverlap);
}

// Called every frame
void APuzzleRoomThreeJoystick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsGrabed)
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

	GiveHapticFeedback();
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
	if (puzzlePathFinding->GetbWasReport())
	{
		return;
	}
	GiveHapticFeedback();
	// 튀어나온 상태이므로 평평한 상태로 돌린 후 초기화
	puzzlePathFinding->MovingTrigger();
	puzzlePathFinding->ResetRegularlyUpCountForResetPuzzle();
	puzzlePathFinding->ResetThisPuzzle();
}

void APuzzleRoomThreeJoystick::ControlByPlayerHand()
{
	UMotionControllerComponent* playerHand;

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

void APuzzleRoomThreeJoystick::GiveHapticFeedback()
{
	if (player->heldComponentLeft == this->grabComp)
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
	}
	else
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
	}
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

