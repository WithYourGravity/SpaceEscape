// Fill out your copyright notice in the Description page of Project Settings.


#include "Doors.h"
#include "Components/TimelineComponent.h"
#include "DoorButton.h"
#include "EngineUtils.h"
//#include "Components/BoxComponent.h"

// Sets default values
ADoors::ADoors()
{
 	// Set this actor to call Tick() every frame.  You can t	urn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("doorMesh"));
	doorMesh->SetupAttachment(RootComponent);
	doorMesh->SetRelativeLocation(FVector(0, 0, 0));
	doorMesh->SetRelativeScale3D(FVector(1, 4.625f, 2.4f));
	initLoc = GetActorLocation();
	//triggerboxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("triggerboxComp"));
	//triggerboxComp->SetupAttachment(RootComponent);
	//triggerboxComp->SetRelativeLocation(FVector(0, 0, 0));
	//triggerboxComp->SetRelativeScale3D(FVector(5, 10, 1));
}

// Called when the game starts or when spawned
void ADoors::BeginPlay()
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
			//triggerboxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoors::OnTriggeredOverlap);
			//triggerboxComp->OnComponentEndOverlap.AddDynamic(this, &ADoors::OnTriggeredEndOverlap);
		}
	}
	
}

// Called every frame
void ADoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	curveTimeline.TickTimeline(DeltaTime);
}

void ADoors::ChangeDoorOverlaping()
{	
	bIsOpenOverlaping == true ? Open() : Close();	
}

void ADoors::Open()
{
	startPoint = GetActorLocation();
	endPoint = startPoint + FVector(0, yOffset, 0);
	curveTimeline.PlayFromStart();
	bIsOpenOverlaping = false;	
	UE_LOG(LogTemp, Warning, TEXT("Overlapped : Open Door"))
}

void ADoors::Close()
{
	if (bIsOpenOverlaping == true) return;
	startPoint = GetActorLocation();	
	endPoint = initLoc;
	curveTimeline.PlayFromStart();
	bIsOpenOverlaping = true;
	UE_LOG(LogTemp, Warning, TEXT("EndOverlapped : Close Door"))
	
}

void ADoors::TimeLineProgress(float val)
{
	FVector newLoc = FMath::Lerp(startPoint, endPoint, val);
	SetActorLocation(newLoc);	
}

/*
void ADoors::OnTriggeredOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsOpenOverlaping = true;
	Open();
}

void ADoors::OnTriggeredEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(bIsOpenOverlaping == false)
	{
		Close();
	}
}
*/
