// Fill out your copyright notice in the Description page of Project Settings.


#include "QuatRotatedDoor.h"
#include "Components/TimelineComponent.h"
#include "DoorButton.h"
#include "EngineUtils.h"

// Sets default values
AQuatRotatedDoor::AQuatRotatedDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("doorMesh"));
	doorMesh->SetupAttachment(RootComponent);
	doorMesh->SetRelativeLocation(FVector(0, 0, 0));
	doorMesh->SetRelativeScale3D(FVector(1, 4.625f, 2.4f));
	initLoc = GetActorLocation();
}

// Called when the game starts or when spawned
void AQuatRotatedDoor::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<ADoorButton> it(GetWorld()); it; ++it)
	{
		ADoorButton* db = *it;

		//db->openDoorDele.BindUFunction(this, FName("ChangeDoorOverlaping"));
		db->openDoorDele.AddUFunction(this, FName("ChangeDoorOverlaping"));

		initLoc = GetActorLocation();
		if (curveFloat)
		{
			FOnTimelineFloat TimeLineProgress;
			TimeLineProgress.BindUFunction(this, FName("TimeLineProgress"));
			curveTimeline.AddInterpFloat(curveFloat, TimeLineProgress);
		}
	}	
}

// Called every frame
void AQuatRotatedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	curveTimeline.TickTimeline(DeltaTime);
}

void AQuatRotatedDoor::ChangeDoorOverlaping()
{
	bIsOpenOverlaping == true ? Open() : Close();
}

void AQuatRotatedDoor::Open()
{
	startPoint = GetActorLocation();
	endPoint = startPoint + FVector(xOffset, 0, 0);
	curveTimeline.PlayFromStart();
	bIsOpenOverlaping = false;
	//UE_LOG(LogTemp, Warning, TEXT("Overlapped : Open Door"))
}

void AQuatRotatedDoor::Close()
{
	if (bIsOpenOverlaping == true) return;
	startPoint = GetActorLocation();
	endPoint = initLoc;
	curveTimeline.PlayFromStart();
	bIsOpenOverlaping = true;
	//UE_LOG(LogTemp, Warning, TEXT("EndOverlapped : Close Door"))
}

void AQuatRotatedDoor::TimeLineProgress(float val)
{
	FVector newLoc = FMath::Lerp(startPoint, endPoint, val);
	SetActorLocation(newLoc);
}