// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoLaserWheel.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "PuzzleRoomTwoLaser.h"
#include "PuzzleRoomTwoLaserLever.h"
#include "Kismet/GameplayStatics.h"

APuzzleRoomTwoLaserWheel::APuzzleRoomTwoLaserWheel()
{
	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	//SetRootComponent(boxComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);
	meshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_Wheel.SM_Wheel'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(RootComponent);
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

	// 플레이어 캐싱
	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	// 맵에 있는 모든 laser를 캐싱해온다
	UGameplayStatics::GetAllActorsOfClass(this, APuzzleRoomTwoLaser::StaticClass(), laserArray);
	currentLaser = Cast<APuzzleRoomTwoLaser>(laserArray[arrayIndex]);
	// 맵에 있는 lever를 캐싱해온다
	laserLever = Cast<APuzzleRoomTwoLaserLever>(UGameplayStatics::GetActorOfClass(this, APuzzleRoomTwoLaserLever::StaticClass()));
	// 레버가 당겨지면 laser index가 증가한다
	laserLever->laserLaverDele.BindUFunction(this, FName("ChangeLaserIndex"));
	// 플레이어 grab delegate 바인딩
	grabComp->onGrabbedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));
	grabComp->onDroppedDelegate.AddUFunction(this, TEXT("ChangeIsGrabed"));
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
	UE_LOG(LogTemp, Warning, TEXT("it works!"));
	// 레이저 회전 부분
	currentLaser->SetActorRotation(GetActorRotation() + FRotator(0, 30, 0));

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

	if (!bRecordOnce)
	{
		startVector = (handLocation - GetActorLocation()).GetSafeNormal();
		bRecordOnce = true;

	}

	FVector afterVector = (handLocation - GetActorLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(startVector, afterVector)));

	//FRotator rot = GetActorRotation();
	//rot.Yaw = degree;
	startVector = (handLocation - GetActorLocation()).GetSafeNormal();
	//rot.Yaw = rot.Yaw + degree;

	AddActorLocalRotation(FRotator(0, degree, 0));
	//SetActorRotation(rot);
	UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
}
