// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreeMorse.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
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
	buttonComp->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	leverBodyComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverBodyComp"));
	leverBodyComp->SetupAttachment(RootComponent);
	leverBodyComp->SetCollisionProfileName(TEXT("NoCollision"));
	leverComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverComp"));
	leverComp->SetupAttachment(leverBodyComp);
	leverComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	screenComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("screenComp"));
	screenComp->SetupAttachment(RootComponent);
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(leverComp);
}

// Called when the game starts or when spawned
void APuzzleRoomThreeMorse::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	grabComp->onDroppedDelegate.BindUFunction(this, TEXT("ChangeIsGrabed"));
	grabComp->onGrabbedDelegate.BindUFunction(this, TEXT("ChangeIsGrabed"));
}

// Called every frame
void APuzzleRoomThreeMorse::Tick(float DeltaTime)
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

// 플레이어 손에 잡혔을 때 레버 이동시키는 함수
void APuzzleRoomThreeMorse::ControlByPlayerHand()
{
	FVector afterVector = (player->rightHandMesh->GetComponentLocation() - leverComp->GetComponentLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(leverComp->GetUpVector(), afterVector)));
	FRotator rot = leverComp->GetRelativeRotation();
	rot.Roll = degree;
	leverComp->SetRelativeRotation(rot);

	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
	//UE_LOG(LogTemp, Warning, TEXT("bisGrabis %d"), bIsGrabed);
}	

// 플레이어 손에 잡혔는지 확인하는 함수
void APuzzleRoomThreeMorse::ChangeIsGrabed()
{
	bIsGrabed = !bIsGrabed;
}

