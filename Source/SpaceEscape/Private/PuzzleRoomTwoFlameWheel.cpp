// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoFlameWheel.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "PuzzleRoomTwoFlame.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APuzzleRoomTwoFlameWheel::APuzzleRoomTwoFlameWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	soundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("soundComp"));
	soundComp->SetupAttachment(RootComponent);
	soundComp->SetAutoActivate(false);

	ConstructorHelpers::FObjectFinder<USoundWave>tempSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/WheelSound.WheelSound'"));
	if (tempSound.Succeeded())
	{
		soundComp->SetSound(tempSound.Object);
	}

	Tags.Add(FName("Sense"));
	meshComp->ComponentTags.Add(FName("Sense.R2"));
}

// Called when the game starts or when spawned
void APuzzleRoomTwoFlameWheel::BeginPlay()
{
	Super::BeginPlay();

	// 맵에 있는 모든 laser를 캐싱해온다
	UGameplayStatics::GetAllActorsOfClass(this, APuzzleRoomTwoFlame::StaticClass(), flameArray);
	selectedFlame = Cast<APuzzleRoomTwoFlame>(flameArray[arrayIndex]);

	// 플레이어 grab delegate 바인딩
	grabComp->onGrabbedDelegate.AddUFunction(this, FName("WhenPlayerGrab"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("WhenPlayerUnGrab"));

	// 플레이어 캐싱
	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

// Called every frame
void APuzzleRoomTwoFlameWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsGrabed)
	{
		ControlByPlayerHand();
	}
}

void APuzzleRoomTwoFlameWheel::WhenPlayerGrab()
{
	bIsGrabed = true;
}

void APuzzleRoomTwoFlameWheel::WhenPlayerUnGrab()
{
	bIsGrabed = false;
	bRecordOnce = false;
	bsoundPlayOnce = false;
}

void APuzzleRoomTwoFlameWheel::ControlByPlayerHand()
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

		// 플레이어쪽 벡터를 구해서 내적한 뒤 예각 둔각인지
		playerDirection = player->GetActorLocation() - GetActorLocation();

	}

	FVector afterVector = (handLocation - GetActorLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(startVector, afterVector)));

	// 두 벡터를 외적해서 수직인 벡터를 구해 좌우 방향을 정해준다
	FVector crossVec = FVector::CrossProduct(startVector, afterVector);

	float degreeForDir = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(crossVec, playerDirection)));

	if (degreeForDir > 90)
	{
		degree *= -1;
	}

	// 휠 돌리기
	AddActorLocalRotation(FRotator(0, degree, 0));
	
	// 휠 돌린 만큼 불꽃 줄어들게(함수 호출하기)
	selectedFlame->CloseValve(degree);

	startVector = (handLocation - GetActorLocation()).GetSafeNormal();

	// degree값에 따라 사운드 재생
	if (degree > 1.5 && !bsoundPlayOnce)
	{
		bsoundPlayOnce = true;
		soundComp->Play();
	}

	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
	//UE_LOG(LogTemp, Warning, TEXT("currentLaserRoll is %f"), currentLaserRoll);
}

