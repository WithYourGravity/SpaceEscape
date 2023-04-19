// Fill out your copyright notice in the Description page of Project Settings.


#include "Doors.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values
ADoors::ADoors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	triggerboxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("triggerboxComp"));
	triggerboxComp->SetupAttachment(RootComponent);
	triggerboxComp->SetRelativeLocation(FVector(-138, 0, 0));
	triggerboxComp->SetRelativeScale3D(FVector(1, 10, 1));
	leftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftDoorMesh"));
	leftDoorMesh->SetupAttachment(RootComponent);
	leftDoorMesh->SetRelativeLocation(FVector(0, 0, 0));
	leftDoorMesh->SetRelativeScale3D(FVector(1, 4.625f, 2.4f));
	//rightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightDoorMesh"));
	//rightDoorMesh->SetupAttachment(RootComponent);
	//rightDoorMesh->SetRelativeLocation(FVector(0, 0, 0));
	//rightDoorMesh->SetRelativeScale3D(FVector(1, 4.625f, 2.4f));
	
		
}

// Called when the game starts or when spawned
void ADoors::BeginPlay()
{
	Super::BeginPlay();	
	
	if(curveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimeLineProgress"));
		curveTimeline.AddInterpFloat(curveFloat, TimeLineProgress);		
		triggerboxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoors::OnTriggeredOverlap);
		triggerboxComp->OnComponentEndOverlap.AddDynamic(this, &ADoors::OnTriggeredEndOverlap);		
	}
}

// Called every frame
void ADoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	curveTimeline.TickTimeline(DeltaTime);
}

void ADoors::OnTriggeredOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	Open();
}

void ADoors::OnTriggeredEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Close();	
}

void ADoors::Open()
{
	startPoint = GetActorLocation();
	curveTimeline.PlayFromStart();
	//leftDoorMesh->AddRelativeLocation(FVector(0, slideDist * -1, 0));
	//rightDoorMesh->AddRelativeLocation(FVector(0, slideDist, 0));
	UE_LOG(LogTemp, Warning, TEXT("Overlapped : Open Door"))
	
}

void ADoors::Close()
{
	endPoint = GetActorLocation() + FVector(0, yOffset, 0);
	curveTimeline.ReverseFromEnd();
	endPoint.Y += yOffset;
	//this->leftDoorMesh->SetRelativeLocation(FVector(0, slideDist, 0));
	//this->rightDoorMesh->SetRelativeLocation(FVector::ZeroVector);
	UE_LOG(LogTemp, Warning, TEXT("EndOverlapped : Close Door"))
	
}

void ADoors::TimeLineProgress(float val)
{
	FVector newLoc = FMath::Lerp(startPoint, endPoint, val);
	SetActorLocation(newLoc);
}
