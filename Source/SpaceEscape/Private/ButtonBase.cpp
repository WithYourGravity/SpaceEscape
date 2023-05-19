// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonBase.h"

#include "EscapePlayer.h"
#include "Components/BoxComponent.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"

// Sets default values
AButtonBase::AButtonBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	buttonBodyComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("buttonBodyComp"));
	SetRootComponent(buttonBodyComp);
	buttonBodyComp->SetRelativeScale3D(FVector(0.5));
	buttonBodyComp->SetCollisionProfileName(TEXT("NoCollision"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempButtonBodyMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/switchers/source/switchers_bigbutton.switchers_bigbutton'"));
	if (tempButtonBodyMesh.Succeeded())
	{
		buttonBodyComp->SetStaticMesh(tempButtonBodyMesh.Object);
	}

	buttonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("buttonComp"));
	buttonComp->SetupAttachment(buttonBodyComp);
	buttonComp->SetCollisionProfileName(TEXT("NoCollision"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempButtonMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/switchers/source/switchers_button.switchers_button'"));
	if (tempButtonMesh.Succeeded())
	{
		buttonComp->SetStaticMesh(tempButtonMesh.Object);
	}

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxCollision->SetupAttachment(buttonBodyComp);
	boxCollision->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	boxCollision->SetBoxExtent(FVector(8.f));
	boxCollision->SetRelativeLocation(FVector(64.f, -6.f, 0));

	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base>tempHaptic(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/LTG/UI/HF_TouchFeedback.HF_TouchFeedback'"));
	if (tempHaptic.Succeeded())
	{
		hapticFeedback = tempHaptic.Object;
	}
}

// Called when the game starts or when spawned
void AButtonBase::BeginPlay()
{
	Super::BeginPlay();

	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AButtonBase::BeginOverlap);
	boxCollision->OnComponentEndOverlap.AddDynamic(this, &AButtonBase::EndOverlap);

	startLoc = buttonComp->GetRelativeLocation();
	endLoc = startLoc + FVector(0, 0, -5.f);
}

// Called every frame
void AButtonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtonBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 버튼 오버랩 연속되지 않게 딜레이 처리
	if (bBeginOverlapDelay)
	{
		return;
	}
	bBeginOverlapDelay = true;
	FTimerHandle delayHandle;
	GetWorldTimerManager().SetTimer(delayHandle, FTimerDelegate::CreateLambda([&]()
		{
			bBeginOverlapDelay = false;
		}), 0.2f, false);

	// 버튼 들어가게
	buttonComp->SetRelativeLocation(endLoc);
	
	// 손 콜리전 검출해서 왼손 오른손 진동처리 가능
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherComp->GetName());

	// 손과 오버랩되면 진동 울리게 처리
	auto player = Cast<AEscapePlayer>(OtherActor);
	if (OtherComp->GetName().Contains("left"))
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
	}
	else
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
	}
	// 오버라이딩할 버튼 트리거 함수
	ButtonTriggered();
}

void AButtonBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	buttonComp->SetRelativeLocation(startLoc);
	// 오버라이딩할 버튼 엔드 함수
	ButtonEnded();
}

void AButtonBase::ButtonTriggered()
{
}

void AButtonBase::ButtonEnded()
{
}

