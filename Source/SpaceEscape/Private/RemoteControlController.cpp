// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoteControlController.h"

#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "RemoteControlObject.h"

// Sets default values
ARemoteControlController::ARemoteControlController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	tabletMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TabletMeshComp"));
	SetRootComponent(tabletMeshComp);
	buttonGoMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonGoMeshComp"));
	buttonGoMeshComp->SetupAttachment(RootComponent);
	buttonBackMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBackMeshComp"));
	buttonBackMeshComp->SetupAttachment(RootComponent);
	buttonLeftMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLeftMeshComp"));
	buttonLeftMeshComp->SetupAttachment(RootComponent);
	buttonRightMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonRightMeshComp"));
	buttonRightMeshComp->SetupAttachment(RootComponent);

	buttonGoCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("buttonGoCollision"));
	buttonGoCollision->SetupAttachment(buttonGoMeshComp);
	buttonBackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("buttonBackCollision"));
	buttonBackCollision->SetupAttachment(buttonBackMeshComp);
	buttonLeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("buttonLeftCollision"));
	buttonLeftCollision->SetupAttachment(buttonLeftMeshComp);
	buttonRightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("buttonRightCollision"));
	buttonRightCollision->SetupAttachment(buttonRightMeshComp);

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

