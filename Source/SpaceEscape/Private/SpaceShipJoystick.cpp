// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipJoystick.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "MotionControllerComponent.h"
#include "SpaceShip.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpaceShipJoystick::ASpaceShipJoystick()
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
	baseMeshComp->SetVisibility(false);

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

	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base>tempHaptic(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/LTG/UI/HF_TouchFeedback.HF_TouchFeedback'"));
	if (tempHaptic.Succeeded())
	{
		hapticFeedback = tempHaptic.Object;
	}

	Tags.Add(FName("Sense"));
	stickMeshComp->ComponentTags.Add(FName("Sense.R4"));
}

// Called when the game starts or when spawned
void ASpaceShipJoystick::BeginPlay()
{
	Super::BeginPlay();

	stickMeshComp->SetMassOverrideInKg(FName(NAME_None), 10, true);

	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	ship = Cast<ASpaceShip>(UGameplayStatics::GetActorOfClass(this, ASpaceShip::StaticClass()));

	constraintComp->SetConstrainedComponents(baseMeshComp, FName("None"), stickMeshComp, FName("None"));

	grabComp->onGrabbedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));
	grabComp->onDroppedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));

	stickPosComp->OnComponentBeginOverlap.AddDynamic(this, &ASpaceShipJoystick::StickOnOverlap);
	stickPosComp->OnComponentEndOverlap.AddDynamic(this, &ASpaceShipJoystick::StickEndOverlap);
}

// Called every frame
void ASpaceShipJoystick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGrabed)
	{
		ControlByPlayerHand();
	}
}

void ASpaceShipJoystick::ChangeIsGrabed()
{
	bIsGrabed = !bIsGrabed;
}

void ASpaceShipJoystick::StickOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetOwner() != this)
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
	GetWorldTimerManager().SetTimer(stickHandle, FTimerDelegate::CreateUObject(this, &ASpaceShipJoystick::RotateFunction, otherCompNameForTimer), GetWorld()->DeltaTimeSeconds, true, 0);
}

void ASpaceShipJoystick::StickEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetOwner() != this)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(stickHandle);
}

void ASpaceShipJoystick::ControlByPlayerHand()
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

void ASpaceShipJoystick::RotateFunction(char componentName)
{
	if(ship->GetActorRotation().Yaw > 88 && ship->GetActorRotation().Yaw < 92)
	{
		return;
	}

	if (componentName == 'f')
	{
		
	}
	else if (componentName == 'b')
	{
		
	}
	else if (componentName == 'l')
	{
		ship->AddActorLocalRotation(FRotator(0, 0.1f, 0));
	}
	else if (componentName == 'r')
	{
		ship->AddActorLocalRotation(FRotator(0, -0.1f, 0));
	}
}

