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

	// �ʿ� �ִ� ��� laser�� ĳ���ؿ´�
	UGameplayStatics::GetAllActorsOfClass(this, APuzzleRoomTwoFlame::StaticClass(), flameArray);
	selectedFlame = Cast<APuzzleRoomTwoFlame>(flameArray[arrayIndex]);

	// �÷��̾� grab delegate ���ε�
	grabComp->onGrabbedDelegate.AddUFunction(this, FName("WhenPlayerGrab"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("WhenPlayerUnGrab"));

	// �÷��̾� ĳ��
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

	// ó�� ��ġ�� �� �� ����
	if (!bRecordOnce)
	{
		startVector = (handLocation - GetActorLocation()).GetSafeNormal();
		bRecordOnce = true;

		// �÷��̾��� ���͸� ���ؼ� ������ �� ���� �а�����
		playerDirection = player->GetActorLocation() - GetActorLocation();

	}

	FVector afterVector = (handLocation - GetActorLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(startVector, afterVector)));

	// �� ���͸� �����ؼ� ������ ���͸� ���� �¿� ������ �����ش�
	FVector crossVec = FVector::CrossProduct(startVector, afterVector);

	float degreeForDir = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(crossVec, playerDirection)));

	if (degreeForDir > 90)
	{
		degree *= -1;
	}

	// �� ������
	AddActorLocalRotation(FRotator(0, degree, 0));
	
	// �� ���� ��ŭ �Ҳ� �پ���(�Լ� ȣ���ϱ�)
	selectedFlame->CloseValve(degree);

	startVector = (handLocation - GetActorLocation()).GetSafeNormal();

	// degree���� ���� ���� ���
	if (degree > 1.5 && !bsoundPlayOnce)
	{
		bsoundPlayOnce = true;
		soundComp->Play();
	}

	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
	//UE_LOG(LogTemp, Warning, TEXT("currentLaserRoll is %f"), currentLaserRoll);
}

