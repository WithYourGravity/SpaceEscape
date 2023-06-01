// Fill out your copyright notice in the Description page of Project Settings.


#include "Magazine.h"
#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "Gun.h"
#include "MotionControllerComponent.h"
#include "Components/AudioComponent.h"
#include "Components/Image.h"
#include "Sound/SoundCue.h"

// Sets default values
AMagazine::AMagazine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	magazineMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("magazineMeshComp"));
	SetRootComponent(magazineMeshComp);
	magazineMeshComp->SetSimulatePhysics(true);
	magazineMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Pistol/Pistol_Cube_008.Pistol_Cube_008'"));
	if (tempMesh.Succeeded())
	{
		magazineMeshComp->SetStaticMesh(tempMesh.Object);
		magazineMeshComp->SetRelativeScale3D(FVector(0.36f));
	}

	bulletMeshComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bulletMeshComp1"));
	bulletMeshComp1->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempBulletMesh1(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Pistol/Pistol_Cylinder_005.Pistol_Cylinder_005'"));
	if (tempBulletMesh1.Succeeded())
	{
		bulletMeshComp1->SetStaticMesh(tempBulletMesh1.Object);
		bulletMeshComp1->SetCollisionProfileName(FName("NoCollision"));
	}

	bulletMeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bulletMeshComp2"));
	bulletMeshComp2->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempBulletMesh2(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Pistol/Pistol_Cylinder_002.Pistol_Cylinder_002'"));
	if (tempBulletMesh2.Succeeded())
	{
		bulletMeshComp2->SetStaticMesh(tempBulletMesh2.Object);
		bulletMeshComp2->SetCollisionProfileName(FName("NoCollision"));
	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(magazineMeshComp);
	grabComp->grabType = EGrabType::SNAP;
	grabComp->SetRelativeLocation(FVector(-5, 0, -14));
	grabComp->SetRelativeRotation(FRotator(90, 0, 0));

	ConstructorHelpers::FObjectFinder<USoundCue> tempMagazineSoundCue(TEXT("/Script/Engine.SoundCue'/Game/MilitaryWeapSilver/Sound/Rifle/Cues/Rifle_Reload_Cue.Rifle_Reload_Cue'"));
	if (tempMagazineSoundCue.Succeeded())
	{
		magazineSoundCue = tempMagazineSoundCue.Object;
	}

	magazineAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("magazineAudioComp"));
	magazineAudioComp->bAutoActivate = false;
	magazineAudioComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMagazine::BeginPlay()
{
	Super::BeginPlay();
	
	magazineMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AMagazine::OnBeginOverlap);
	magazineMeshComp->SetGenerateOverlapEvents(true);

	grabComp->onGrabbedDelegate.AddUFunction(this, FName("OnGrabbed"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("OnDropped"));

	currentBulletCount = maxBulletCount;
}

// Called every frame
void AMagazine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOverlapGun && gun && gun->player && gun->magazine && grabComp->bIsHeld)
	{
		auto playerHand = grabComp->GetHeldByHand() == EControllerHand::Right ? gun->player->rightHand : gun->player->leftHand;
		FVector handLocation = playerHand->GetComponentLocation();
		FVector magazineOverlapLocation = gun->gunMeshComp->GetSocketLocation(FName("MagazineOverlapSocket"));
		
		float dir =  FVector::DotProduct(handLocation - magazineOverlapLocation, gun->gunMeshComp->GetSocketLocation(FName("MagazineSocket")) - magazineOverlapLocation);

		if (FVector::Dist(magazineOverlapLocation, handLocation) >= 20.0f && dir < 0)
		{
			grabComp->TryGrab(playerHand);
			bIsOverlapGun = false;
			magazineMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
			if (gun)
			{
				gun->magazine = nullptr;
				gun = nullptr;
			}
		}
		else
		{
			GrabMagazine();
		}
	}
}

void AMagazine::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (grabComp->bIsHeld)
	{
		gun = Cast<AGun>(OtherActor);
		if (gun != nullptr && gun->bIsOnGrabbed)
		{
			gun->magazine = this;
			AttachToComponent(gun->gunMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("MagazineOverlapSocket"));
			magazineMeshComp->SetSimulatePhysics(false);
			magazineMeshComp->SetCollisionProfileName(FName("NoCollision"));
			bIsOverlapGun = true;
		}
	}
}

void AMagazine::OnGrabbed()
{
}

void AMagazine::OnDropped()
{
	if (gun)
	{
		magazineMeshComp->SetSimulatePhysics(false);
		grabComp->grabType = EGrabType::NONE;
		
		if (grabValue >= 1.0f)
		{
			gun->magazine = this;
			gun->bOnReloading = false;
			gun->bDoReloading = false;
			AttachToComponent(gun->gunMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("MagazineSocket"));
			magazineMeshComp->SetCollisionProfileName(FName("NoCollision"));

			for (int32 i = maxBulletCount - 1; i >= maxBulletCount -  currentBulletCount; i--)
			{
				gun->gunAmmoImage[i]->SetColorAndOpacity(FLinearColor(0.015f, 0.25f, 0.904f));
			}
			
		}
		else
		{
			gun->DropMagazine();
		}
	}
}

void AMagazine::GrabMagazine()
{
	float dist;
	if (grabComp->GetHeldByHand() == EControllerHand::Right)
	{
		dist = FVector::DotProduct(gun->player->rightHand->GetComponentLocation() - grabComp->GetComponentLocation(), grabComp->GetForwardVector());
	}
	else
	{
		dist = FVector::DotProduct(gun->player->leftHand->GetComponentLocation() - grabComp->GetComponentLocation(), grabComp->GetForwardVector());
	}

	FVector startPos = gun->gunMeshComp->GetSocketLocation(FName("MagazineOverlapSocket"));
	FVector endPos = gun->gunMeshComp->GetSocketLocation(FName("MagazineSocket"));

	grabValue += dist * 0.1f;

	grabValue = FMath::Clamp(grabValue, 0.0f, 1.0f);

	SetActorLocation(FMath::Lerp<FVector>(startPos, endPos, grabValue));

	if (grabValue >= 1.0f)
	{
		if (!bIsPlayMagazineSound)
		{
			bIsPlayMagazineSound = true;

			if (magazineSoundCue && magazineAudioComp->IsValidLowLevelFast())
			{
				magazineAudioComp->SetSound(magazineSoundCue);
				magazineAudioComp->Play();
			}
		}
	}
	else
	{
		bIsPlayMagazineSound = false;
	}
}
