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
#include "GunAmmoOthersideWidget.h"
#include "GunAmmoWidget.h"
#include "MotionControllerComponent.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "Haptics/HapticFeedbackEffect_Base.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	gunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunMeshComp"));
	SetRootComponent(gunMeshComp);
	gunMeshComp->SetSimulatePhysics(true);
	gunMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/YSY/Assets/Pistol/Gun.Gun'"));
	if (tempMesh.Succeeded())
	{
		gunMeshComp->SetStaticMesh(tempMesh.Object);
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

	gunAmmoRightWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("gunAmmoRightWidgetComp"));
	gunAmmoRightWidgetComp->SetupAttachment(RootComponent);
	gunAmmoRightWidgetComp->SetRelativeLocation(FVector(-2.7f, 5.0f, -8.0f));
	gunAmmoRightWidgetComp->SetRelativeRotation(FRotator(0, 90, 0));
	gunAmmoRightWidgetComp->SetRelativeScale3D(FVector(0.05f));

	gunAmmoLeftWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("gunAmmoLeftWidgetComp"));
	gunAmmoLeftWidgetComp->SetupAttachment(RootComponent);
	gunAmmoLeftWidgetComp->SetRelativeLocation(FVector(-2.7f, -5.0f, -8.0f));
	gunAmmoLeftWidgetComp->SetRelativeRotation(FRotator(0, -90, 0));
	gunAmmoLeftWidgetComp->SetRelativeScale3D(FVector(0.05f));

	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> tempHapticEffect(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/LTG/UI/HF_TouchFeedback.HF_TouchFeedback'"));
	if (tempHapticEffect.Succeeded())
	{
		grabHapticEffect = tempHapticEffect.Object;
	}

	// Sound
	ConstructorHelpers::FObjectFinder<USoundCue> tempFireSoundCue(TEXT("/Script/Engine.SoundCue'/Game/MilitaryWeapSilver/Sound/Pistol/Cues/PistolA_Fire_Cue.PistolA_Fire_Cue'"));
	if (tempFireSoundCue.Succeeded())
	{
		fireSoundCue = tempFireSoundCue.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> tempReloadMagInsertSoundCue(TEXT("/Script/Engine.SoundCue'/Game/MilitaryWeapSilver/Sound/Pistol/Cues/Pistol_ReloadMagInsert_Cue.Pistol_ReloadMagInsert_Cue'"));
	if (tempReloadMagInsertSoundCue.Succeeded())
	{
		reloadMagInsertSoundCue = tempReloadMagInsertSoundCue.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> tempReloadSlideRackSoundCue(TEXT("/Script/Engine.SoundCue'/Game/MilitaryWeapSilver/Sound/Pistol/Cues/Pistol_ReloadSlideRack_Cue.Pistol_ReloadSlideRack_Cue'"));
	if (tempReloadSlideRackSoundCue.Succeeded())
	{
		reloadSlideRackSoundCue = tempReloadSlideRackSoundCue.Object;
	}

	fireAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("fireAudioComp"));
	fireAudioComp->bAutoActivate = false;
	fireAudioComp->SetupAttachment(muzzleLocation);

	reloadAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("reloadAudioComp"));
	reloadAudioComp->bAutoActivate = false;
	reloadAudioComp->SetupAttachment(slideGrabComp);

	Tags.Add(FName("Sense"));
	gunMeshComp->ComponentTags.Add(FName("Sense.Always"));
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

	gunAmmoRightUI = Cast<UGunAmmoWidget>(gunAmmoRightWidgetComp->GetUserWidgetObject());
	gunAmmoLeftUI = Cast<UGunAmmoOthersideWidget>(gunAmmoLeftWidgetComp->GetUserWidgetObject());
	
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo1);
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo2);
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo3);
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo4);
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo5);
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo6);
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo7);
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo8);
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo9);
	gunAmmoImage.Add(gunAmmoRightUI->img_ammo10);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo1o);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo2o);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo3o);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo4o);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo5o);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo6o);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo7o);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo8o);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo9o);
	gunAmmoImage.Add(gunAmmoLeftUI->img_ammo10o);
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
				subSystem->RemoveMappingContext(IMC_WeaponLeft);
			}
			else
			{
				subSystem->AddMappingContext(IMC_WeaponLeft, 0);
				subSystem->RemoveMappingContext(IMC_WeaponRight);
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

	if (player && player->bIsOverlapGunStorage && player->storedGun == nullptr)
	{
		gunMeshComp->SetSimulatePhysics(false);
		gunMeshComp->AttachToComponent(player->gunStorageComp, FAttachmentTransformRules::KeepWorldTransform);
		gunMeshComp->SetWorldLocation(player->gunStorageComp->GetComponentLocation());
		player->storedGun = this;

		gunMeshComp->SetCollisionProfileName(FName("NoCollision"));

		SetActorHiddenInGame(true);

		// haptic effect
		if (grabHapticEffect)
		{
			GetWorld()->GetFirstPlayerController()->PlayHapticEffect(grabHapticEffect, grabComp->GetHeldByHand());
		}
	}
}

void AGun::Fire(float fireAlpha)
{
	// Trigger Rotation
	FRotator startRot = FRotator(0.0f, 0.0f, 0.0f);
	FRotator endRot = FRotator(-30.0f, 0.0f, 0.0f);
	//triggerSceneComp->SetRelativeRotation(FMath::Lerp<FRotator>(startRot, endRot, fireAlpha));
	triggerSceneComp->SetRelativeRotation(FQuat::Slerp(startRot.Quaternion(), endRot.Quaternion(), fireAlpha));

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

			// spawn effect
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), muzzleFlashComp, loc, rot, true);

			// spawn bullet
			GetWorld()->SpawnActor<ABullet>(bulletFactory, loc, rot);
			magazine->FireBullet();

			if (magazine->GetCurrentBulletCount() == 0)
			{
				magazine->bulletMeshComp1->SetVisibility(false);
				OpenSlider();
				if (reloadMagInsertSoundCue && fireAudioComp->IsValidLowLevelFast())
				{
					fireAudioComp->SetSound(reloadMagInsertSoundCue);
					fireAudioComp->Play();
				}
			}
			else if (magazine->GetCurrentBulletCount() == 1)
			{
				magazine->bulletMeshComp2->SetVisibility(false);
			}

			// haptic effect
			if (grabHapticEffect)
			{
				GetWorld()->GetFirstPlayerController()->PlayHapticEffect(grabHapticEffect, grabComp->GetHeldByHand());
			}

			if (fireSoundCue && fireAudioComp->IsValidLowLevelFast())
			{
				fireAudioComp->SetSound(fireSoundCue);
				fireAudioComp->Play();
			}

			gunAmmoImage[magazine->maxBulletCount - magazine->GetCurrentBulletCount() - 1]->SetColorAndOpacity(FLinearColor::White);
		}
	}
}

void AGun::DropMagazine()
{
	if (magazine)
	{
		for (int32 i = magazine->maxBulletCount - 1; i >= magazine->maxBulletCount - magazine->GetCurrentBulletCount(); i--)
		{
			gunAmmoImage[i]->SetColorAndOpacity(FLinearColor::White);
		}

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

				magazine->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				magazine->magazineMeshComp->SetSimulatePhysics(true);
				magazine->grabComp->grabType = EGrabType::SNAP;
				magazine->magazineMeshComp->SetCollisionProfileName(FName("PuzzleObjectPreset"));
				magazine->gun = nullptr;
				magazine = nullptr;

				GetWorld()->GetTimerManager().ClearTimer(magazineTimer);
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
	params.AddIgnoredComponent(player->rightAim);
	params.AddIgnoredComponent(player->leftAim);
	params.AddIgnoredComponent(Cast<UPrimitiveComponent>(player->dieWidgetComp));
	params.AddIgnoredComponent(Cast<UPrimitiveComponent>(player->infoWidgetComp));

	//bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startPos, endPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(5.0f), params);

	float distance = 0;
	// 충돌이 발생하면
	if (bHit)
	{
		// 충돌한 지점에 Crosshair 표시
		crosshair->crosshairComp->SetVisibility(true);
		crosshair->SetActorLocation(hitInfo.ImpactPoint);
		distance = hitInfo.Distance;
	}
	// 그렇지 않으면
	else
	{
		crosshair->crosshairComp->SetVisibility(false);
	}

	crosshair->SetActorScale3D(FVector(FMath::Max<float>(1.0f, distance * 0.002f)));

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

	if (!bOnReloading)
	{
		if (reloadMagInsertSoundCue && reloadAudioComp->IsValidLowLevelFast())
		{
			reloadAudioComp->SetSound(reloadMagInsertSoundCue);
			reloadAudioComp->Play();
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

	if (!bDoReloading)
	{
		if (reloadSlideRackSoundCue && reloadAudioComp->IsValidLowLevelFast())
		{
			reloadAudioComp->SetSound(reloadSlideRackSoundCue);
			reloadAudioComp->Play();
		}
	}

	bOnReloading = false;
	bDoReloading = true;
}
