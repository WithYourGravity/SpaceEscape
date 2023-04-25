// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"

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
}

// Called when the game starts or when spawned
void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	grabComp->onDroppedDelegate.BindUFunction(this, TEXT("ChangeIsGrabed"));
	grabComp->onGrabbedDelegate.BindUFunction(this, TEXT("ChangeIsGrabed"));
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

	if (rot.Roll < -2)
	{
		rot.Roll += 5;
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
	bool bIsLeftHand;
	FVector handLocation;
	if (player && player->heldComponentLeft == this->grabComp)
	{
		bIsLeftHand = true;
		handLocation = player->leftHandMesh->GetComponentLocation();
	}
	else if (player && player->heldComponentRight == this->grabComp)
	{
		bIsLeftHand = false;
		handLocation = player->rightHandMesh->GetComponentLocation();
	}

	if (!bRecordOnce)
	{
		startVector = (handLocation - coverMeshComp->GetComponentLocation()).GetSafeNormal();
		bRecordOnce = true;
	}
	FVector afterVector = (handLocation - coverMeshComp->GetComponentLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(startVector, afterVector)));
	FRotator rot = coverMeshComp->GetRelativeRotation();
	rot.Roll = FMath::Clamp(degree * 1.75f, 0, 90.f) * -1;
	coverMeshComp->SetRelativeRotation(rot);

	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
	//UE_LOG(LogTemp, Warning, TEXT("bisGrabis %d"), bIsGrabed);
}

void AAmmoBox::ChangeIsGrabed()
{
	bIsGrabed = !bIsGrabed;
}

