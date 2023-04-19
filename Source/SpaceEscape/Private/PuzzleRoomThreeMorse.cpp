// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorse.h"

#include "EscapePlayer.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
APuzzleRoomThreeMorse::APuzzleRoomThreeMorse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SetRootComponent(sceneComp);
	buttonBodyComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("buttonBodyComp"));
	buttonBodyComp->SetupAttachment(RootComponent);
	buttonBodyComp->SetCollisionProfileName(TEXT("NoCollision"));

	buttonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("buttonComp"));
	buttonComp->SetupAttachment(buttonBodyComp);
	leverBodyComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverBodyComp"));
	leverBodyComp->SetupAttachment(RootComponent);
	leverBodyComp->SetCollisionProfileName(TEXT("NoCollision"));

	leverComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverComp"));
	leverComp->SetupAttachment(leverBodyComp);
	leverCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("leverCollision"));
	leverCollision->SetupAttachment(leverComp);
	screenComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("screenComp"));
	screenComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APuzzleRoomThreeMorse::BeginPlay()
{
	Super::BeginPlay();
	leverCollision->OnComponentBeginOverlap.AddDynamic(this, &APuzzleRoomThreeMorse::OnOverlap);
}

// Called every frame
void APuzzleRoomThreeMorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	CheckIsGrabed();
	if (!bIsGrabed)
	{
		LeverShouldBeZero();
	}
	else
	{
		ControlByPlayerHand();
	}
	*/
}

// ���� ƽ���� ����ġ�� �̵���Ű�� �Լ�
void APuzzleRoomThreeMorse::LeverShouldBeZero()
{
	FRotator rot = leverComp->GetRelativeRotation();

	if (rot.Roll > 5)
	{
		rot.Roll -= 5;
		leverComp->SetRelativeRotation(rot);
	}
	else
	{
		rot.Roll = 0;
		leverComp->SetRelativeRotation(rot);
	} 
}

// �÷��̾� �տ� ������ �� ���� �̵���Ű�� �Լ�
void APuzzleRoomThreeMorse::ControlByPlayerHand()
{
	if (player)
	{
		if (player->bIsGrabbed)
		{
			FRotator rot = leverComp->GetRelativeRotation();
			leverComp->SetRelativeRotation(rot);
		}
	}
}

// �÷��̾� �տ� �������� Ȯ���ϴ� �Լ�
void APuzzleRoomThreeMorse::CheckIsGrabed()
{
	if (player)
	{
		bIsGrabed = player->bIsGrabbed;
	}
}

// ���� �����̺κ� ������ ������ ����� �Լ�
void APuzzleRoomThreeMorse::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<AEscapePlayer>(OtherActor);
	if (player)
	{
		
	}
}

