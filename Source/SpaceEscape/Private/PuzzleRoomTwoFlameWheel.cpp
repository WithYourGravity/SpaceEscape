// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomTwoFlameWheel.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "PuzzleRoomTwoFlame.h"
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
}

// Called when the game starts or when spawned
void APuzzleRoomTwoFlameWheel::BeginPlay()
{
	Super::BeginPlay();

	// �ʿ� �ִ� ��� laser�� ĳ���ؿ´�
	UGameplayStatics::GetAllActorsOfClass(this, APuzzleRoomTwoFlame::StaticClass(), flameArray);
	selectedFlame = Cast<APuzzleRoomTwoFlame>(flameArray[arrayIndex]);

	// �÷��̾� grab delegate ���ε�
	grabComp->onGrabbedDelegate.AddUFunction(this, FName("ChangeIsGrabed"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("ChangeIsGrabed"));

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

void APuzzleRoomTwoFlameWheel::ChangeIsGrabed()
{
	bIsGrabed = !bIsGrabed;
	bRecordOnce = false;
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

	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
	//UE_LOG(LogTemp, Warning, TEXT("currentLaserRoll is %f"), currentLaserRoll);
}

