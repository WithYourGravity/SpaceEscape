// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrewDriver.h"
#include "Components/BoxComponent.h"
#include "Screw.h"
#include "GrabComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AScrewDriver::AScrewDriver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Screw Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(0.8, 0.7, 0.8));
	boxComp->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewMesh"));
	meshComp->SetupAttachment(RootComponent);	
	ConstructorHelpers::FObjectFinder<UStaticMesh> dMesh(TEXT("/Script/Engine.StaticMesh'/Game/Deko_MatrixDemo/Apartment/Meshes/SM_ScrewDriver_A01_N1.SM_ScrewDriver_A01_N1'"));
	if(dMesh.Succeeded())
	{
		meshComp->SetStaticMesh(dMesh.Object);
		meshComp->SetRelativeLocation(FVector(-9, 0, 0));
		meshComp->SetRelativeRotation(FRotator(0, 90, 180));
		meshComp->SetRelativeScale3D(FVector(1));
		meshComp->SetWorldRotation(FRotator(0, 90, 0));
	}
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::FREE;
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	arrowComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AScrewDriver::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AScrewDriver::AttachtoScrew);	
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AScrewDriver::DettachFromScrew);
}

// Called every frame
void AScrewDriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	//UE_LOG(LogTemp, Warning, TEXT("Tick Attach : isAttaching  = %d"), isAttaching)
	if (isAttaching == true)
	{
		deltaRot = GetActorRotation().Roll - prevDriverRot;
		//UE_LOG(LogTemp, Warning, TEXT("delatRot = %f"), deltaRot)
	}	
}

void AScrewDriver::AttachtoScrew(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AScrew* attachedScrew = Cast<AScrew>(OtherActor);
	if (attachedScrew != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OtherActor = %s"), *OtherActor->GetName())
		isAttaching = true;
		//닿았을 때 최초 회전값
		prevDriverRot = GetActorRotation().Roll;
	}
}

void AScrewDriver::DettachFromScrew(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 나사가 다 돌아가서 빠져나왔다면, 나사 physics 켜준다
	AScrew* attachedScrew = Cast<AScrew>(OtherActor);
	if (attachedScrew != nullptr)
	{ 
		isAttaching = false;	
	}
}
