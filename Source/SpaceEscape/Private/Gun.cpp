// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "GrabComponent.h"
#include "Components/BoxComponent.h"

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

	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("grabComp"));
	grabComp->SetupAttachment(gunMeshComp);
	grabComp->SetRelativeLocation(FVector(-46.66f, 35.52f, 42.67f));
	grabComp->SetRelativeRotation(FRotator(50, -90, 0));
	grabComp->grabType = EGrabType::SNAP;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	grabComp->onGrabbedDelegate.BindUFunction(this, FName("OnGrabbed"));
	grabComp->onDroppedDelegate.BindUFunction(this, FName("OnDropped"));

	// Crosshair ��ü ����
	/*if (crosshairFactory)
	{
		crosshair = GetWorld()->SpawnActor<ACrosshair>(crosshairFactory);
	}*/
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::OnGrabbed()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGrabbed"));
	EnableInput(GetWorld()->GetFirstPlayerController());

	// Enhanced Input ���ó��
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
		}
	}
}

void AGun::OnDropped()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDropped"));
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
		}
	}
}

void AGun::BindToInput()
{
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		auto InputSystem = CastChecked<UEnhancedInputComponent>(InputComponent);
		if (InputSystem)
		{
			InputSystem->BindAction(IA_FireLeft, ETriggerEvent::Started, this, &AGun::FireLeft);
			InputSystem->BindAction(IA_FireRight, ETriggerEvent::Started, this, &AGun::FireRight);
		}
		
	}
}

void AGun::FireLeft()
{

}

void AGun::FireRight()
{

}

// �Ÿ��� ���� Crosshair ũ�Ⱑ ���� ���̰� �Ѵ�.
void AGun::DrawCrosshair()
{
	//// ������
	//FVector startPos = rightAim->GetComponentLocation();
	//// ����
	//FVector endPos = startPos + rightAim->GetForwardVector() * fireDistance;

	//// �浹 üũ
	//FHitResult hitInfo;
	//bool bHit = HitTest(startPos, endPos, hitInfo);

	//float distance = 0;
	//// �浹�� �߻��ϸ�
	//if (bHit)
	//{
	//	// �浹�� ������ Crosshair ǥ��
	//	crosshair->crosshairComp->SetVisibility(true);
	//	crosshair->SetActorLocation(hitInfo.Location);
	//	distance = hitInfo.Distance;
	//}
	//// �׷��� ������
	//else
	//{
	//	crosshair->crosshairComp->SetVisibility(false);
	//}

	//crosshair->SetActorScale3D(FVector(FMath::Max<float>(1, distance * crosshairScale)));

	//// Crosshair �� ī�޶� �ٶ󺸵��� ó��
	//FVector direction = crosshair->GetActorLocation() - vrCamera->GetComponentLocation();
	//crosshair->SetActorRotation(direction.Rotation());
}
