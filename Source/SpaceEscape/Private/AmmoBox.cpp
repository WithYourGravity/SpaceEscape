// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("boxMeshComp"));
	SetRootComponent(boxMeshComp);
	boxMeshComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempBoxMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_Box.SM_Box'"));
    if (tempBoxMesh.Succeeded())
    {
		boxMeshComp->SetStaticMesh(tempBoxMesh.Object);
    }

	coverMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("coverMeshComp"));
	coverMeshComp->SetupAttachment(RootComponent);
	coverMeshComp->SetRelativeLocation(FVector(0, -19.f, 21.f));
	coverMeshComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempCoverMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_BoxCover.SM_BoxCover'"));
    if (tempCoverMesh.Succeeded())
    {
		coverMeshComp->SetStaticMesh(tempCoverMesh.Object);
    }

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(coverMeshComp);
	grabComp->grabType = EGrabType::LEVER;

	Tags.Add(FName("Sense"));
}

// Called when the game starts or when spawned
void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	grabComp->onDroppedDelegate.AddUFunction(this, TEXT("OnDroped"));
	grabComp->onGrabbedDelegate.AddUFunction(this, TEXT("OnGrabed"));
}

// Called every frame
void AAmmoBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsGrabed)
	{
		ControlByPlayerHand();
	}
	else
	{
		BoxShouldBeClosed();
	}
}

void AAmmoBox::BoxShouldBeClosed()
{
	FRotator rot = coverMeshComp->GetRelativeRotation();

	if (rot.Roll == 0)
	{
		return;
	}

	if (rot.Roll < -2)
	{
		rot.Roll += 4;
		coverMeshComp->SetRelativeRotation(rot);
	}
	else
	{
		rot.Roll = 0;
		coverMeshComp->SetRelativeRotation(rot);
		bRecordOnce = false;
	}
}

void AAmmoBox::ControlByPlayerHand()
{
	// 잡은 손이 왼손인지 오른손인지 판단
	FVector handLocation;
	if (player && player->heldComponentLeft == this->grabComp)
	{
		// 손 바뀌면 바뀐 손과의 벡터차를 구해서 스타트벡터에 더해줌
		handLocation = player->leftHandMesh->GetComponentLocation();
		if (!bRecordLeftOnce)
		{
			bRecordLeftOnce = true;
			bRecordRightOnce = false;
			deltaVector = (handLocation - coverMeshComp->GetComponentLocation()).GetSafeNormal() - afterVector;
			startVector += deltaVector;
		}
	}
	else
	{
		handLocation = player->rightHandMesh->GetComponentLocation();
		if (!bRecordRightOnce)
		{
			bRecordRightOnce = true;
			bRecordLeftOnce = false;
			deltaVector = (handLocation - coverMeshComp->GetComponentLocation()).GetSafeNormal() - afterVector;
			startVector += deltaVector;
		}
	}

	// 기준 벡터 처음 잡을때 한번만 초기화
	if (!bRecordOnce)
	{
		startVector = (handLocation - coverMeshComp->GetComponentLocation()).GetSafeNormal();
		bRecordOnce = true;
		//UE_LOG(LogTemp, Error, TEXT("Start Vector Recorded"));
	}

	// 현재 손 위치와 박스커버 사이의 벡터를 구해서 기준벡터와 내적
	afterVector = (handLocation - coverMeshComp->GetComponentLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(startVector, afterVector)));

	// 사이각 만큼 박스 Roll값 변경하여 열어주기
	FRotator rot = coverMeshComp->GetRelativeRotation();
	rot.Roll = FMath::Clamp(degree * 1.5f, 0, 90.f) * -1;
	coverMeshComp->SetRelativeRotation(rot);

	//UE_LOG(LogTemp, Warning, TEXT("degree is : %f"), degree);
}

void AAmmoBox::OnGrabed()
{
	if (!bIsGrabed)
	{
		bIsGrabed = true;
		//UE_LOG(LogTemp, Error, TEXT("On Grabed"));
	}
}

void AAmmoBox::OnDroped()
{
	if (bIsGrabed)
	{
		bIsGrabed = false;
		//UE_LOG(LogTemp, Error, TEXT("On Droped"));
	}
}

