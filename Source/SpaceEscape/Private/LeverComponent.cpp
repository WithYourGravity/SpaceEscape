// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverComponent.h"

#include "EscapePlayer.h"
#include "GrabComponent.h"
// Sets default values for this component's properties
ULeverComponent::ULeverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	this->SetRelativeScale3D(FVector(0.5f));
	this->SetRelativeRotation(FRotator(0, 0, -90.f));
	leverBodyComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverBodyMeshComp"));
	leverBodyComp->SetupAttachment(this);
	leverBodyComp->SetCollisionProfileName(TEXT("NoCollision"));
	//leverBodyComp->SetRelativeScale3D(FVector(0.3f));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempLeverBodyMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/switchers/source/switchers_lever.switchers_lever'"));
	if (tempLeverBodyMesh.Succeeded())
	{
		leverBodyComp->SetStaticMesh(tempLeverBodyMesh.Object);
	}

	leverComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leverMeshComp"));
	leverComp->SetupAttachment(leverBodyComp);
	leverComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	leverComp->SetRelativeLocation(FVector(0, -5.f, 0));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempLeverMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/switchers/source/switchers_handle.switchers_handle'"));
	if (tempLeverMesh.Succeeded())
	{
		leverComp->SetStaticMesh(tempLeverMesh.Object);
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(leverComp);
	grabComp->grabType = EGrabType::LEVER;
}


// Called when the game starts
void ULeverComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	grabComp->onDroppedDelegate.BindUFunction(this, TEXT("ChangeIsGrabed"));
	grabComp->onGrabbedDelegate.BindUFunction(this, TEXT("ChangeIsGrabed"));
}


// Called every frame
void ULeverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsGrabed)
	{
		LeverShouldBeZero();
	}
	else
	{
		ControlByPlayerHand();
	}
}

// ���� ƽ���� ����ġ�� �̵���Ű�� �Լ�
void ULeverComponent::LeverShouldBeZero()
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
		bIsTriggered = false;
		bDoOnce = false;
	}
}

// �÷��̾� �տ� ������ �� ���� �̵���Ű�� �Լ�
void ULeverComponent::ControlByPlayerHand()
{
	if (!bDoOnce)
	{
		startVector = (player->rightHandMesh->GetComponentLocation() - leverComp->GetComponentLocation()).GetSafeNormal();
		bDoOnce = true;
	}
	FVector afterVector = (player->rightHandMesh->GetComponentLocation() - leverComp->GetComponentLocation()).GetSafeNormal();
	float degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(startVector, afterVector)));
	FRotator rot = leverComp->GetRelativeRotation();
	rot.Roll = FMath::Clamp(degree * 1.75f, 0, 90.f);
	leverComp->SetRelativeRotation(rot);

	if (degree > 45)
	{
		bIsTriggered = true;
	}
	//UE_LOG(LogTemp, Warning, TEXT("degree is %f"), degree);
	//UE_LOG(LogTemp, Warning, TEXT("bisGrabis %d"), bIsGrabed);
}

// �÷��̾� �տ� �������� Ȯ���ϴ� �Լ�
void ULeverComponent::ChangeIsGrabed()
{
	bIsGrabed = !bIsGrabed;
}

// ���� �۵����� Ȯ���ϴ� �Լ�
bool ULeverComponent::GetIsTriggered()
{
	return bIsTriggered;
}
