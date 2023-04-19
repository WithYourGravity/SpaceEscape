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
#include "Camera/CameraComponent.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;
	boxComp->SetSimulatePhysics(true);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

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
	slideGrabComp->grabType = EGrabType::SNAP;

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("muzzleLocation"));
	muzzleLocation->SetupAttachment(gunMeshComp);
	muzzleLocation->SetRelativeLocation(FVector(-47.0f, -140.0f, 13.33f));
	muzzleLocation->SetRelativeRotation(FRotator(-30, -90, 0));
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
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isOnGrabbed)
	{
		DrawCrosshair();
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
			Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->grabbedGun = this;
			isOnGrabbed = true;
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
			Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->grabbedGun = nullptr;
			isOnGrabbed = false;
		}
	}
}

void AGun::Fire()
{
	FVector loc = muzzleLocation->GetComponentLocation();
	FRotator rot = muzzleLocation->GetComponentRotation();
	GetWorld()->SpawnActor<AActor>(bulletFactory, loc, rot);
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

	crosshair->SetActorScale3D(FVector(FMath::Max<float>(1, distance *crosshairScale)));

	// Crosshair 가 카메라를 바라보도록 처리
	FVector direction = crosshair->GetActorLocation() - Cast<AEscapePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->vrCamera->GetComponentLocation();
	crosshair->SetActorRotation(direction.Rotation());
}
