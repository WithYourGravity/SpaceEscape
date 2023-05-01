// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EscapePlayer.h"
#include "GrabComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Crosshair.h"
#include "Magazine.h"
#include "Bullet.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;
	boxComp->SetSimulatePhysics(true);
	boxComp->SetBoxExtent(FVector(2.5f, 15.0f, 10.0f));
	boxComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));

	gunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Pistol/Gun.Gun'"));
	if (tempMesh.Succeeded())
	{
		gunMeshComp->SetStaticMesh(tempMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(0.0f, 9.0f, -1.0f));
		gunMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
		gunMeshComp->SetRelativeScale3D(FVector(0.36f));
	}
	
	gunSlideMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunSlideMeshComp"));
	gunSlideMeshComp->SetupAttachment(gunMeshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSlideMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Pistol/GunSlider.GunSlider'"));
	if (tempSlideMesh.Succeeded())
	{
		gunSlideMeshComp->SetStaticMesh(tempSlideMesh.Object);
	}

	triggerSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("triggerSceneComp"));
	triggerSceneComp->SetupAttachment(gunMeshComp);
	triggerSceneComp->SetRelativeLocation(FVector(18.0f, 0.0f, 13.0f));

	gunTriggerMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunTriggerMeshComp"));
	gunTriggerMeshComp->SetupAttachment(triggerSceneComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempTriggerMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Pistol/Pistol_Cube_006.Pistol_Cube_006'"));
	if (tempTriggerMesh.Succeeded())
	{
		gunTriggerMeshComp->SetStaticMesh(tempTriggerMesh.Object);
		gunTriggerMeshComp->SetRelativeLocation(FVector(-18.0f, 0.0f, -13.0f));
	}

	

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(gunMeshComp);
	grabComp->SetRelativeLocation(FVector(-2.73f, 0.0f, -3.78f));
	grabComp->SetRelativeRotation(FRotator(75, 0, 0));
	grabComp->grabType = EGrabType::SNAP;

	slideGrabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("slideGrabComp"));
	slideGrabComp->SetupAttachment(gunMeshComp);
	slideGrabComp->SetRelativeLocation(FVector(11.0f, 0.0f, 22.0f));
	slideGrabComp->grabType = EGrabType::GUNSLIDER;

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("muzzleLocation"));
	muzzleLocation->SetupAttachment(gunMeshComp);
	muzzleLocation->SetRelativeLocation(FVector(70.0f, 0.0f, 25.0f));

	magazineBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("magazineBoxComp"));
	magazineBoxComp->SetupAttachment(gunMeshComp);
	magazineBoxComp->SetRelativeLocation(FVector(-5.0f, 0.0f, -16.0f));
	magazineBoxComp->SetBoxExtent(FVector(10, 6, 7));
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	grabComp->onGrabbedDelegate.AddUFunction(this, FName("OnGrabbed"));
	grabComp->onDroppedDelegate.AddUFunction(this, FName("OnDropped"));
	

	// Crosshair 객체 생성
	if (crosshairFactory)
	{
		crosshair = GetWorld()->SpawnActor<ACrosshair>(crosshairFactory);
	}

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	initGunSlideCompLocation = slideGrabComp->GetRelativeLocation();
	initGunSlideMeshLocation = gunSlideMeshComp->GetRelativeLocation();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOnGrabbed)
	{
		DrawCrosshair();

		if (slideGrabComp->bIsGunSlideGrabbed)
		{
			GrabSlider();
		}
		else
		{
			if (bOnReloading && FVector::Dist(slideGrabComp->GetComponentLocation(), initGunSlideCompLocation) >= 0.1f)
			{
				CloseSlider();
			}
		}
	}
}

void AGun::OnGrabbed()
{
	EnableInput(GetWorld()->GetFirstPlayerController());

	// Enhanced Input 사용처리
	auto PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		auto localPlayer = PC->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if (subSystem)
		{
			if (grabComp->GetHeldByHand() == EControllerHand::Right)
			{
				subSystem->AddMappingContext(IMC_WeaponRight, 0);
			}
			else
			{
				subSystem->AddMappingContext(IMC_WeaponLeft, 0);
			}

			if (player)
			{
				player->grabbedGun = this;
			}
			bIsOnGrabbed = true;
		}
	}
}

void AGun::OnDropped()
{
	DisableInput(GetWorld()->GetFirstPlayerController());

	auto PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		auto localPlayer = PC->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if (subSystem)
		{
			if (grabComp->GetHeldByHand() == EControllerHand::Right)
			{
				subSystem->RemoveMappingContext(IMC_WeaponRight);
			}
			else
			{
				subSystem->RemoveMappingContext(IMC_WeaponLeft);
			}

			if (player)
			{
				player->grabbedGun = nullptr;
			}
			
			bIsOnGrabbed = false;
			crosshair->crosshairComp->SetVisibility(false);
		}
	}
}

void AGun::Fire(float fireAlpha)
{
	// Trigger Rotation
	FRotator startRot = FRotator(0.0f, 0.0f, 0.0f);
	FRotator endRot = FRotator(-30.0f, 0.0f, 0.0f);
	triggerSceneComp->SetRelativeRotation(FMath::Lerp<FRotator>(startRot, endRot, fireAlpha));

	if (bFireCompleted)
	{
		return;
	}

	if (fireAlpha >= 0.9f)
	{
		bFireCompleted = true;
	
		if (magazine && magazine->GetCurrentBulletCount() != 0 && bDoReloading)
		{
			FVector loc = muzzleLocation->GetComponentLocation();
			FRotator rot = muzzleLocation->GetComponentRotation();
			GetWorld()->SpawnActor<ABullet>(bulletFactory, loc, rot);
			magazine->FireBullet();

			if (magazine->GetCurrentBulletCount() == 0)
			{
				magazine->bulletMeshComp1->SetVisibility(false);
				OpenSlider();
			}
			else if (magazine->GetCurrentBulletCount() == 1)
			{
				magazine->bulletMeshComp2->SetVisibility(false);
			}
		}
	}
}

void AGun::DropMagazine()
{
	if (magazine)
	{
		GetWorld()->GetTimerManager().SetTimer(magazineTimer, FTimerDelegate::CreateLambda([this]()->void
		{
			currentTime += GetWorld()->DeltaTimeSeconds;

			FVector curPos = magazine->GetActorLocation();
			FVector endPos = gunMeshComp->GetSocketLocation(FName("MagazineDropSocket"));

			curPos = FMath::Lerp<FVector>(curPos, endPos, currentTime / dropMagazineTime);

			magazine->SetActorLocation(curPos);

			if (currentTime >= dropMagazineTime)
			{
				currentTime = 0.0f;
				magazine->SetActorLocation(endPos);
				GetWorld()->GetTimerManager().ClearTimer(magazineTimer);

				magazine->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				magazine->boxComp->SetSimulatePhysics(true);
				magazine->grabComp->grabType = EGrabType::SNAP;
				magazine->boxComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
				magazine->gun = nullptr;
				magazine = nullptr;
			}
		}), 0.02f, true);
	}
}

// 거리에 따라서 Crosshair 크기가 같게 보이게 한다.
void AGun::DrawCrosshair()
{
	// 시작점
	FVector startPos = muzzleLocation->GetComponentLocation();
	// 끝점
	FVector endPos = startPos + muzzleLocation->GetForwardVector() * fireDistance;

	// 충돌 체크
	FHitResult hitInfo;
	FCollisionQueryParams params;
	// 자기자신 무시
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	float distance = 0;
	// 충돌이 발생하면
	if (bHit)
	{
		// 충돌한 지점에 Crosshair 표시
		crosshair->crosshairComp->SetVisibility(true);
		crosshair->SetActorLocation(hitInfo.Location);
		distance = hitInfo.Distance;
	}
	// 그렇지 않으면
	else
	{
		crosshair->crosshairComp->SetVisibility(false);
	}

	crosshair->SetActorScale3D(FVector(FMath::Max<float>(1, distance * crosshairScale)));

	// Crosshair 가 카메라를 바라보도록 처리
	if (player)
	{
		FVector direction = crosshair->GetActorLocation() - player->vrCamera->GetComponentLocation();
		crosshair->SetActorRotation(direction.Rotation());
	}
}

void AGun::GrabSlider()
{
	float dist;
	if (slideGrabComp->GetHeldByHand() == EControllerHand::Right)
	{
		dist = FVector::DotProduct(player->rightHand->GetComponentLocation() - slideGrabComp->GetComponentLocation(),slideGrabComp->GetForwardVector());
	}
	else
	{
		dist = FVector::DotProduct(player->leftHand->GetComponentLocation() - slideGrabComp->GetComponentLocation(), slideGrabComp->GetForwardVector());
	}
	
	if (dist < 0)
	{
		if (FVector::Dist(initGunSlideCompLocation, slideGrabComp->GetRelativeLocation()) < reloadSliderDistance)
		{
			slideGrabComp->SetWorldLocation(slideGrabComp->GetComponentLocation() + slideGrabComp->GetForwardVector() * dist * 0.5f);

			gunSlideMeshComp->SetWorldLocation(gunSlideMeshComp->GetComponentLocation() + slideGrabComp->GetForwardVector() * dist * 0.5f);
		}
	}

	bOnReloading = true;
	bDoReloading = false;
}

void AGun::OpenSlider()
{
	slideGrabComp->SetRelativeLocation(initGunSlideCompLocation + FVector(reloadSliderDistance * -1.0f, 0.0f, 0.0f));
	gunSlideMeshComp->SetRelativeLocation(initGunSlideMeshLocation + FVector(reloadSliderDistance * -1.0f, 0.0f, 0.0f));

	bOnReloading = false;
	bDoReloading = false;
}

void AGun::CloseSlider()
{
	slideGrabComp->SetRelativeLocation(initGunSlideCompLocation);
	gunSlideMeshComp->SetRelativeLocation(initGunSlideMeshLocation);

	bDoReloading = true;
}
