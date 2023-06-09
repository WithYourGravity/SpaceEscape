// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoteControlController.h"

#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "EscapePlayer.h"
#include "RemoteControlObject.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"

// Sets default values
ARemoteControlController::ARemoteControlController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	tabletMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TabletMeshComp"));
	SetRootComponent(tabletMeshComp);
	tabletMeshComp->SetRelativeScale3D(FVector(1.5f));
	tabletMeshComp->SetSimulatePhysics(true);
	tabletMeshComp->SetGenerateOverlapEvents(false);
	tabletMeshComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Yeni/Assets/ModularSciFiOffice/Meshes/Props/SM_Tablet_01.SM_Tablet_01'"));
    if (tempMesh.Succeeded())
    {
		tabletMeshComp->SetStaticMesh(tempMesh.Object);
    }

	buttonGoMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonGoMeshComp"));
	buttonGoMeshComp->SetupAttachment(RootComponent);
	buttonGoMeshComp->SetRelativeLocationAndRotation(FVector(-25.f, 17.f, 1.f), FRotator(0, 270, 0));
	buttonGoMeshComp->SetRelativeScale3D(FVector(0.05f));
	buttonGoMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	buttonBackMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBackMeshComp"));
	buttonBackMeshComp->SetupAttachment(RootComponent);
	buttonBackMeshComp->SetRelativeLocationAndRotation(FVector(-19.f, 17.f, 1.f), FRotator(0, 90, 0));
	buttonBackMeshComp->SetRelativeScale3D(FVector(0.05f));
	buttonBackMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	buttonLeftMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLeftMeshComp"));
	buttonLeftMeshComp->SetupAttachment(RootComponent);
	buttonLeftMeshComp->SetRelativeLocationAndRotation(FVector(-12.f, 17.f, 1.f), FRotator(0, 180, 0));
	buttonLeftMeshComp->SetRelativeScale3D(FVector(0.05f));
	buttonLeftMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	buttonRightMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonRightMeshComp"));
	buttonRightMeshComp->SetupAttachment(RootComponent);
	buttonRightMeshComp->SetRelativeLocation(FVector(-5.f, 17.f, 1.f));
	buttonRightMeshComp->SetRelativeScale3D(FVector(0.05f));
	buttonRightMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempButtonIMG(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_TabletButton.SM_TabletButton'"));
    if (tempButtonIMG.Succeeded())
    {
		buttonGoMeshComp->SetStaticMesh(tempButtonIMG.Object);
		buttonBackMeshComp->SetStaticMesh(tempButtonIMG.Object);
		buttonLeftMeshComp->SetStaticMesh(tempButtonIMG.Object);
		buttonRightMeshComp->SetStaticMesh(tempButtonIMG.Object);
    }

	buttonGoCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonGoCollision"));
	buttonGoCollision->SetupAttachment(buttonGoMeshComp);
	buttonGoCollision->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.f));
	buttonGoCollision->SetRelativeLocation(FVector(0, 0, -20.f));
	buttonGoCollision->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	buttonBackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonBackCollision"));
	buttonBackCollision->SetupAttachment(buttonBackMeshComp);
	buttonBackCollision->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.f));
	buttonBackCollision->SetRelativeLocation(FVector(0, 0, -20.f));
	buttonBackCollision->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	buttonLeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonLeftCollision"));
	buttonLeftCollision->SetupAttachment(buttonLeftMeshComp);
	buttonLeftCollision->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.f));
	buttonLeftCollision->SetRelativeLocation(FVector(0, 0, -20.f));
	buttonLeftCollision->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));
	buttonRightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonRightCollision"));
	buttonRightCollision->SetupAttachment(buttonRightMeshComp);
	buttonRightCollision->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.f));
	buttonRightCollision->SetRelativeLocation(FVector(0, 0, -20.f));
	buttonRightCollision->SetCollisionProfileName(TEXT("PuzzleButtonPreset"));

	tabletScreenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TabletScreenComp"));
	tabletScreenComp->SetupAttachment(RootComponent);
	tabletScreenComp->SetRelativeScale3D(FVector(0.27f, 0.19f, 1.f));
	tabletScreenComp->SetRelativeLocation(FVector(-15.f, 10.6f, 0.8f));
	tabletScreenComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base>tempHaptic(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/LTG/UI/HF_TouchFeedback.HF_TouchFeedback'"));
	if (tempHaptic.Succeeded())
	{
		hapticFeedback = tempHaptic.Object;
	}

	Tags.Add(FName("Sense"));
	tabletMeshComp->ComponentTags.Add(FName("Sense.R1"));
}

// Called when the game starts or when spawned
void ARemoteControlController::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ARemoteControlObject> it(GetWorld()); it; ++it)
	{
		ARemoteControlObject* ro = *it;
		remoteObject = ro;
	}

	buttonGoCollision->OnComponentBeginOverlap.AddDynamic(this, &ARemoteControlController::OnOverlap);
	buttonGoCollision->OnComponentEndOverlap.AddDynamic(this, &ARemoteControlController::EndOverlap);
	buttonBackCollision->OnComponentBeginOverlap.AddDynamic(this, &ARemoteControlController::OnOverlap);
	buttonBackCollision->OnComponentEndOverlap.AddDynamic(this, &ARemoteControlController::EndOverlap);
	buttonLeftCollision->OnComponentBeginOverlap.AddDynamic(this, &ARemoteControlController::OnOverlap);
	buttonLeftCollision->OnComponentEndOverlap.AddDynamic(this, &ARemoteControlController::EndOverlap);
	buttonRightCollision->OnComponentBeginOverlap.AddDynamic(this, &ARemoteControlController::OnOverlap);
	buttonRightCollision->OnComponentEndOverlap.AddDynamic(this, &ARemoteControlController::EndOverlap);
}

// Called every frame
void ARemoteControlController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARemoteControlController::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 컴포넌트 이름 6번째 letter 가져와서 구분처리
	FString compName;
	compName.AppendChar(OverlappedComponent->GetName()[6]);
	remoteObject->ControlManager(compName);

	auto player = Cast<AEscapePlayer>(OtherActor);
	if (player && OtherComp->GetName().Contains("left"))
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
	}
	else
	{
		player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
	}
}

void ARemoteControlController::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 컴포넌트 이름 6번째 letter 가져와서 구분처리
	FString compName;
	compName.AppendChar(OverlappedComponent->GetName()[6]);
	if (compName == "G" || compName == "B")
	{
		remoteObject->StopMove();
	}
	else
	{
		remoteObject->StopTurn();
	}
}

