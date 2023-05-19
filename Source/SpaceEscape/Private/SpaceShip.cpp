// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"

#include "EscapePlayer.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	shipMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("shipMeshComp"));
	SetRootComponent(shipMeshComp);
	shipMeshComp->SetCollisionProfileName(FName("NoCollision"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_SpaceShip.SM_SpaceShip'"));
    if (tempMesh.Succeeded())
    {
		shipMeshComp->SetStaticMesh(tempMesh.Object);
    }

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetBoxExtent(FVector(600.f, 300.f, 100.f));
	boxComp->SetRelativeLocation(FVector(300.f, 100.f, 100.f));
	boxComp->SetRelativeRotation(FRotator(0, -30.f, 0));

	forLocComp = CreateDefaultSubobject<USphereComponent>(TEXT("forLocComp"));
	forLocComp->SetupAttachment(RootComponent);
	forLocComp->SetCollisionProfileName(FName("NoCollision"));
	forLocComp->SetRelativeLocation(FVector(270.f, 0, 250.f));
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ASpaceShip::OnOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &ASpaceShip::EndOverlap);

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpaceShip::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ��ư��� ó��
	auto pl = Cast<AEscapePlayer>(OtherActor);
	if (pl)
	{
		bReadyToBoarding = true;
	}
}

void ASpaceShip::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ��� ��� ó��
	auto pl = Cast<AEscapePlayer>(OtherActor);
	if (pl)
	{
		bReadyToBoarding = false;
	}
}

void ASpaceShip::BoardingShip()
{
	// ����� ž�¹����� �ƴ϶�� ����
	if (!bReadyToBoarding)
	{
		return;
	}

	// �̹� Ÿ�� �ִٸ�
	if (bIsBoarding)
	{
		// �÷��̾� �̵� �Ѱ�
		player->moveMode = tempSaveMoveMode;
		player->GetCharacterMovement()->MaxWalkSpeed = 400;

		// �ٱ����� �̵���Ų��
		player->SetActorLocation(GetActorLocation() + FVector(0, -400.f, 0));
		player->SetActorRotation(FRotator::ZeroRotator);
	}

	// �÷��̾� �̵����� 
	player->GetCharacterMovement()->MaxWalkSpeed = 0;
	tempSaveMoveMode = player->moveMode;
	player->moveMode = EMoveModeState::JOYSTICK;

	// ������ ��ġ�� �̵���Ų��
	player->SetActorLocation(forLocComp->GetComponentLocation());
	player->SetActorRotation(GetActorRotation());
	UE_LOG(LogTemp, Warning, TEXT("pitch : %f, yaw : %f, roll : %f"), GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);

	// Ÿ�� ������ ���
	bIsBoarding = true;
}

