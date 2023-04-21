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
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;
	boxComp->SetSimulatePhysics(true);
	boxComp->SetBoxExtent(FVector(5, 15, 10));
	boxComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));

	gunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Gun/0ae7c8526de44d0ab63e6b5d21341fd2_fbx_Frame_low.0ae7c8526de44d0ab63e6b5d21341fd2_fbx_Frame_low'"));
	if (tempMesh.Succeeded())
	{
		gunMeshComp->SetStaticMesh(tempMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(6.5f, 0.617f, -5.06f));
		gunMeshComp->SetRelativeRotation(FRotator(0, 0, 30));
		gunMeshComp->SetRelativeScale3D(FVector(0.15f));
	}
	
	gunSlideMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunSlideMeshComp"));
	gunSlideMeshComp->SetupAttachment(gunMeshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSlideMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Gun/0ae7c8526de44d0ab63e6b5d21341fd2_fbx_Slide_low.0ae7c8526de44d0ab63e6b5d21341fd2_fbx_Slide_low'"));
	if (tempSlideMesh.Succeeded())
	{
		gunSlideMeshComp->SetStaticMesh(tempSlideMesh.Object);

	}

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(gunMeshComp);
	grabComp->SetRelativeLocation(FVector(-46.66f, 35.52f, 42.67f));
	grabComp->SetRelativeRotation(FRotator(50, -90, 0));
	grabComp->grabType = EGrabType::SNAP;

	slideGrabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("slideGrabComp"));
	slideGrabComp->SetupAttachment(gunMeshComp);
	slideGrabComp->SetRelativeLocation(FVector(-46.66f, 9.76f, 96.42f));
	slideGrabComp->SetRelativeRotation(FRotator(-30, -90, 0));
	slideGrabComp->grabType = EGrabType::GUNSLIDER;

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("muzzleLocation"));
	muzzleLocation->SetupAttachment(gunMeshComp);
	muzzleLocation->SetRelativeLocation(FVector(-47.0f, -140.0f, 13.33f));
	muzzleLocation->SetRelativeRotation(FRotator(-30, -90, 0));

	magazineBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("magazineBoxComp"));
	magazineBoxComp->SetupAttachment(gunMeshComp);
	magazineBoxComp->SetRelativeLocation(FVector(-46.66f, 77.08f, 14.0f));
	magazineBoxComp->SetRelativeRotation(FRotator(0, 0, -30));
	magazineBoxComp->SetBoxExtent(FVector(15, 20, 8));
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	grabComp->onGrabbedDelegate.BindUFunction(this, FName("OnGrabbed"));
	grabComp->onDroppedDelegate.BindUFunction(this, FName("OnDropped"));
	

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
			if (FVector::Dist(slideGrabComp->GetComponentLocation(), initGunSlideCompLocation) >= 0.1f)
			{
				ReleaseSlider();
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

void AGun::Fire()
{
	if (magazine && magazine->GetCurrentBulletCount() != 0)
	{
		FVector loc = muzzleLocation->GetComponentLocation();
		FRotator rot = muzzleLocation->GetComponentRotation();
		GetWorld()->SpawnActor<AActor>(bulletFactory, loc, rot);
		magazine->FireBullet();
	}
}

void AGun::DropMagazine()
{
	if (magazine)
	{
		magazine->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		magazine->boxComp->SetSimulatePhysics(true);
		magazine->grabComp->grabType = EGrabType::SNAP;
		magazine = nullptr;
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
	
	if (FVector::Dist(initGunSlideCompLocation, slideGrabComp->GetRelativeLocation()) < 100.0f && dist < 0)
	{
		slideGrabComp->SetWorldLocation(slideGrabComp->GetComponentLocation() + slideGrabComp->GetForwardVector() * dist * 0.5f);
		
		gunSlideMeshComp->SetWorldLocation(gunSlideMeshComp->GetComponentLocation() + slideGrabComp->GetForwardVector() * dist * 0.5f);
	}
	
}

void AGun::ReleaseSlider()
{
	slideGrabComp->SetRelativeLocation(initGunSlideCompLocation);
	gunSlideMeshComp->SetRelativeLocation(initGunSlideMeshLocation);
}
