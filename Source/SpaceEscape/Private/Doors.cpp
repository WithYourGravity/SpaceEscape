// Fill out your copyright notice in the Description page of Project Settings.


#include "Doors.h"
#include "Components/BoxComponent.h"

// Sets default values
ADoors::ADoors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	leftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftDoorMesh"));
	leftDoorMesh->SetupAttachment(RootComponent);
	leftDoorMesh->SetRelativeLocation(FVector(0, -463, 0));
	leftDoorMesh->SetRelativeScale3D(FVector(1, 4.625f, 2.4f));
	rightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightDoorMesh"));
	rightDoorMesh->SetupAttachment(RootComponent);
	rightDoorMesh->SetRelativeLocation(FVector(0, 0, 0));
	rightDoorMesh->SetRelativeScale3D(FVector(1, 4.625f, 2.4f));
	triggerboxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("triggerboxComp"));
	triggerboxComp->SetupAttachment(RootComponent);
	triggerboxComp->SetRelativeLocation(FVector(-117, -233, -88));
	triggerboxComp->SetRelativeScale3D(FVector(2, 3, 1));
}

// Called when the game starts or when spawned
void ADoors::BeginPlay()
{
	Super::BeginPlay();

	triggerboxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoors::OnTriggeredOverlap);
	triggerboxComp->OnComponentEndOverlap.AddDynamic(this, &ADoors::OnTriggeredEndOverlap);
	
}

// Called every frame
void ADoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	leftDoorMesh->AddRelativeLocation(FVector(0, slideDist * -1, 0));
	rightDoorMesh->AddRelativeLocation(FVector(0, slideDist, 0));
	UE_LOG(LogTemp, Warning, TEXT("Overlapped Door"))

	/*FTimeLine*/
	/*UCurveFloat, 리셀란하우스*/

}

void ADoors::Close()
{
	FTimerHandle closeTimerHandle;
	GetWorldTimerManager().SetTimer(closeTimerHandle, FTimerDelegate::CreateLambda([this]()->void
	{
		this->leftDoorMesh->SetRelativeLocation(FVector::ZeroVector);
		this->rightDoorMesh->SetRelativeLocation(FVector::ZeroVector);
	}
	), 1, false);
	
}
