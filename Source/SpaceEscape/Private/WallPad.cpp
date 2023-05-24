// Fill out your copyright notice in the Description page of Project Settings.


#include "WallPad.h"

#include "EscapePlayer.h"
#include "PuzzleRoomTwoWallPadWidget.h"
#include "RoomManager.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWallPad::AWallPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pad Mesh"));
	SetRootComponent(meshComp);
	meshComp->SetRelativeScale3D(FVector(0.2f));
	meshComp->SetSimulatePhysics(false);
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/sci-fi-tablet/source/Tablet/Tablet.Tablet'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	wallScreenComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Wall Screen Widget"));
	wallScreenComp->SetupAttachment(RootComponent);
	wallScreenComp->SetCollisionProfileName(TEXT("NoCollision"));

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetBoxExtent(FVector(16.f, 32.f, 16.f));
	boxComp->SetRelativeLocation(FVector(15.f, 0, 0));
	boxComp->SetCollisionProfileName(FName("PuzzleButtonPreset"));

	Tags.Add(FName("Sense"));
}

// Called when the game starts or when spawned
void AWallPad::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AWallPad::OnOverlap);
	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));
	gravityWidget = Cast<UPuzzleRoomTwoWallPadWidget>(wallScreenComp->GetWidget());
}

// Called every frame
void AWallPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallPad::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto pl = Cast<AEscapePlayer>(OtherActor);
	if (gravityWidget && pl && !bActiveOnce)
	{
		gravityWidget->GravityActivate();
		bActiveOnce = true;
	}
}

