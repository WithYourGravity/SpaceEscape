// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoLaserWheel.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "PuzzleRoomTwoLaser.h"
#include "PuzzleRoomTwoLaserLever.h"
#include "Kismet/GameplayStatics.h"

APuzzleRoomTwoLaserWheel::APuzzleRoomTwoLaserWheel()
{
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_Wheel.SM_Wheel'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(meshComp);
	grabComp->grabType = EGrabType::LEVER;

	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base>tempHaptic(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/LTG/UI/HF_TouchFeedback.HF_TouchFeedback'"));
	if (tempHaptic.Succeeded())
	{
		hapticFeedback = tempHaptic.Object;
	}
}

void APuzzleRoomTwoLaserWheel::BeginPlay()
{
	Super::BeginPlay();

	
	// �ʿ� �ִ� ��� laser�� ĳ���ؿ´�
	UGameplayStatics::GetAllActorsOfClass(this, APuzzleRoomTwoLaser::StaticClass(), laserArray);
	currentLaser = Cast<APuzzleRoomTwoLaser>(laserArray[arrayIndex]);
	// �ʿ� �ִ� lever�� ĳ���ؿ´�
	laserLever = Cast<APuzzleRoomTwoLaserLever>(UGameplayStatics::GetActorOfClass(this, APuzzleRoomTwoLaserLever::StaticClass()));
	// ������ ������� laser index�� �����Ѵ�
	laserLever->laserLaverDele.BindUFunction(this, FName("ChangeLaserIndex"));
	// �÷��̾� grab delegate ���ε�
	grabComp->onGrabbedDelegate.AddUFunction(this, FName("ChangeIsGrabed"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("ChangeIsGrabed"));

	// �÷��̾� ĳ��
	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

void APuzzleRoomTwoLaserWheel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsGrabed)
	{
		ControlByPlayerHand();
	}
}

void APuzzleRoomTwoLaserWheel::ChangeLaserIndex()
{
	arrayIndex++;
	currentLaser = Cast<APuzzleRoomTwoLaser>(laserArray[arrayIndex % laserArray.Num()]);
}

void APuzzleRoomTwoLaserWheel::ChangeIsGrabed()
{
	bIsGrabed = !bIsGrabed;
	bRecordOnce = false;
}

void APuzzleRoomTwoLaserWheel::ControlByPlayerHand()
{
	bool bIsLeftHand;
	FVector handLocation;
	if (player && player->heldComponentLeft == this->grabComp)
	{
		bIsLeftHand = true;
		handLocation = player->leftHandMesh->GetComponentLocation();
	}
	else
	{
		bIsLeftHand = false;
		handLocation = player->rightHandMesh->GetComponentLocation();
	}

	// ó�� ��ġ�� �� �� ����
	if (!bRecordOnce)
	{
		startVector = (handLocation - GetActorLocation()).GetSafeNormal();
		bRecordOnce = true;

	}

	FVector afterVector = (handLocation - GetActorLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(startVector, afterVector)));
	// �� ���͸� �����ؼ� ������ ���͸� ���� �¿� ������ �����ش�
	FVector crossVec = FVector::CrossProduct(startVector, afterVector);
	if (crossVec.Z < 0)
	{
		degree *= -1;
	}
	// �� ������
	AddActorLocalRotation(FRotator(0, degree, 0));
	// ������ ���� ������
	float currentLaserRoll = currentLaser->laserTopMeshComp->GetRelativeRotation().Roll;
	if (currentLaserRoll <= 90 && currentLaserRoll >= -90)
	{
		currentLaser->laserTopMeshComp->AddLocalRotation(FRotator(0, 0, degree * laserRotateSpeed));
	}

	startVector = (handLocation - GetActorLocation()).GetSafeNormal();

	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
	//UE_LOG(LogTemp, Warning, TEXT("currentLaserRoll is %f"), currentLaserRoll);
}
