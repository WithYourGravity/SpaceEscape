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

	
	// 맵에 있는 모든 laser를 캐싱해온다
	UGameplayStatics::GetAllActorsOfClass(this, APuzzleRoomTwoLaser::StaticClass(), laserArray);
	currentLaser = Cast<APuzzleRoomTwoLaser>(laserArray[arrayIndex]);
	// 맵에 있는 lever를 캐싱해온다
	laserLever = Cast<APuzzleRoomTwoLaserLever>(UGameplayStatics::GetActorOfClass(this, APuzzleRoomTwoLaserLever::StaticClass()));
	// 레버가 당겨지면 laser index가 증가한다
	laserLever->laserLaverDele.BindUFunction(this, FName("ChangeLaserIndex"));
	// 플레이어 grab delegate 바인딩
	grabComp->onGrabbedDelegate.AddUFunction(this, FName("ChangeIsGrabed"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("ChangeIsGrabed"));

	// 플레이어 캐싱
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

	// 처음 위치값 한 번 갱신
	if (!bRecordOnce)
	{
		startVector = (handLocation - GetActorLocation()).GetSafeNormal();
		bRecordOnce = true;

	}

	FVector afterVector = (handLocation - GetActorLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(startVector, afterVector)));
	// 두 벡터를 외적해서 수직인 벡터를 구해 좌우 방향을 정해준다
	FVector crossVec = FVector::CrossProduct(startVector, afterVector);
	if (crossVec.Z < 0)
	{
		degree *= -1;
	}
	// 휠 돌리기
	AddActorLocalRotation(FRotator(0, degree, 0));
	// 레이저 각도 돌리기
	float currentLaserRoll = currentLaser->laserTopMeshComp->GetRelativeRotation().Roll;
	if (currentLaserRoll <= 90 && currentLaserRoll >= -90)
	{
		currentLaser->laserTopMeshComp->AddLocalRotation(FRotator(0, 0, degree * laserRotateSpeed));
	}

	startVector = (handLocation - GetActorLocation()).GetSafeNormal();

	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
	//UE_LOG(LogTemp, Warning, TEXT("currentLaserRoll is %f"), currentLaserRoll);
}
