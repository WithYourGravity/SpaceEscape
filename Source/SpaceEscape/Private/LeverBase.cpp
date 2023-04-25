// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverBase.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"

// Sets default values
ALeverBase::ALeverBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	leverBodyComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverBodyMeshComp"));
	SetRootComponent(leverBodyComp);
	leverBodyComp->SetRelativeScale3D(FVector(0.5f));
	leverBodyComp->SetRelativeRotation(FRotator(0, 0, -90.f));
	leverBodyComp->SetCollisionProfileName(TEXT("NoCollision"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempLeverBodyMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/switchers/source/switchers_lever.switchers_lever'"));
	if (tempLeverBodyMesh.Succeeded())
	{
		leverBodyComp->SetStaticMesh(tempLeverBodyMesh.Object);
	}

	leverComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverMeshComp"));
	leverComp->SetupAttachment(leverBodyComp);
	leverComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	leverComp->SetRelativeLocation(FVector(0, 0, 5.f));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempLeverMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/switchers/source/switchers_handle.switchers_handle'"));
	if (tempLeverMesh.Succeeded())
	{
		leverComp->SetStaticMesh(tempLeverMesh.Object);
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(leverComp);
	grabComp->grabType = EGrabType::LEVER;

	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base>tempHaptic(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/LTG/UI/HF_TouchFeedback.HF_TouchFeedback'"));
	if (tempHaptic.Succeeded())
	{
		hapticFeedback = tempHaptic.Object;
	}
}

// Called when the game starts or when spawned
void ALeverBase::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	grabComp->onDroppedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));
	grabComp->onGrabbedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));
}

// Called every frame
void ALeverBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsGrabed)
	{
		LeverShouldBeZero();
	}
	else
	{
		ControlByPlayerHand();
	}
}

// 레버 틱마다 원위치로 이동시키는 함수
void ALeverBase::LeverShouldBeZero()
{
	FRotator rot = leverComp->GetRelativeRotation();

	if (rot.Roll > 5)
	{
		rot.Roll -= 2;
		leverComp->SetRelativeRotation(rot);
	}
	else
	{
		rot.Roll = 0;
		leverComp->SetRelativeRotation(rot);
		bRecordOnce = false;
		bTriggerOnce = false;
	}
}

// 플레이어 손에 잡혔을 때 레버 이동시키는 함수
void ALeverBase::ControlByPlayerHand()
{
	bool bIsLeftHand;
	FVector handLocation;
	if (player && player->heldComponentLeft == this->grabComp)
	{
		bIsLeftHand = true;
		handLocation = player->leftHandMesh->GetComponentLocation();
	}
	else if (player && player->heldComponentRight == this->grabComp)
	{
		bIsLeftHand = false;
		handLocation = player->rightHandMesh->GetComponentLocation();
	}

	if (!bRecordOnce)
	{
		startVector = (handLocation - leverComp->GetComponentLocation()).GetSafeNormal();
		bRecordOnce = true;
	}
	FVector afterVector = (handLocation - leverComp->GetComponentLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(startVector, afterVector)));
	FRotator rot = leverComp->GetRelativeRotation();
	rot.Roll = FMath::Clamp(degree * 1.75f, 0, 90.f);
	leverComp->SetRelativeRotation(rot);

	if (degree > 45 && !bTriggerOnce)
	{
		LeverTriggered();
		bTriggerOnce = true;
		if (bIsLeftHand)
		{
			player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
		}
		else
		{
			player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
	//UE_LOG(LogTemp, Warning, TEXT("bisGrabis %d"), bIsGrabed);
}

// 플레이어 손에 잡혔는지 확인하는 함수
void ALeverBase::ChangeIsGrabed()
{
	bIsGrabed = !bIsGrabed;
}

// 레버 작동여부 확인하는 함수
void ALeverBase::LeverTriggered()
{
	//UE_LOG(LogTemp, Warning, TEXT("Lever Triggered"));
}
