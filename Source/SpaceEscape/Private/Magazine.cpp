// Fill out your copyright notice in the Description page of Project Settings.


#include "Magazine.h"
#include "GrabComponent.h"
#include "Gun.h"
#include "Components/BoxComponent.h"

// Sets default values
AMagazine::AMagazine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;
	boxComp->SetSimulatePhysics(true);
	boxComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	boxComp->SetBoxExtent(FVector(9, 4, 2));

	magazineMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("magazineMeshComp"));
	magazineMeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Gun/0ae7c8526de44d0ab63e6b5d21341fd2_fbx_Clip_low.0ae7c8526de44d0ab63e6b5d21341fd2_fbx_Clip_low'"));
	if (tempMesh.Succeeded())
	{
		magazineMeshComp->SetStaticMesh(tempMesh.Object);
		magazineMeshComp->SetRelativeScale3D(FVector(0.15f));
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::SNAP;
	grabComp->SetRelativeLocation(FVector(6, 1, 0));
	grabComp->SetRelativeRotation(FRotator(0, 180, 270));
}

// Called when the game starts or when spawned
void AMagazine::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AMagazine::OnBeginOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, 
	&AMagazine::OnEndOverlap);
	boxComp->SetGenerateOverlapEvents(true);

	grabComp->onGrabbedDelegate.BindUFunction(this, FName("OnGrabbed"));
	grabComp->onDroppedDelegate.BindUFunction(this, FName("OnDropped"));

	currentBulletCount = maxBulletCount;
}

// Called every frame
void AMagazine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagazine::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (grabComp->bIsHeld)
	{
		gun = Cast<AGun>(OtherActor);
		if (gun != nullptr && gun->bIsOnGrabbed)
		{
			AttachToComponent(gun->gunMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("MagazineOverlapSocket"));
			bIsOverlapGun = true;
		}
	}
}

void AMagazine::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsOverlapGun)
	{
		bIsOverlapGun = false;
		gun = nullptr;
	}
}

void AMagazine::OnGrabbed()
{

}

void AMagazine::OnDropped()
{
	if (bIsOverlapGun)
	{
		boxComp->SetSimulatePhysics(false);
		if (gun)
		{
			gun->magazine = this;
			AttachToComponent(gun->gunMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("MagazineSocket"));
			grabComp->grabType = EGrabType::NONE;
		}
	}
}
