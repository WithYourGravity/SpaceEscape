// Fill out your copyright notice in the Description page of Project Settings.


#include "Screw.h"

#include "EscapePlayer.h"
#include "ScrewDriver.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "GrabComponent.h"

AScrew::AScrew()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Screw Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(1, 1.6, 1));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewMesh"));
	meshComp->SetupAttachment(RootComponent);
		
	ConstructorHelpers::FObjectFinder<UStaticMesh> smesh(TEXT("/Script/Engine.StaticMesh'/Game/Yeni/Assets/screw/Screw_low.Screw_low'"));
	if(smesh.Succeeded())
	{
		meshComp->SetStaticMesh(smesh.Object);
		meshComp->SetRelativeScale3D(FVector(0.1));
		meshComp->SetRelativeLocationAndRotation(FVector(-7, 0, 0), FRotator(0, 90, -90));
		meshComp->SetWorldRotation(FRotator(0, 0, 0));
	}
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	arrowComp->SetupAttachment(RootComponent);
	//grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	//grabComp->SetupAttachment(RootComponent);
	//grabComp->grabType = EGrabType::FREE;
}

void AScrew::BeginPlay()
{
	Super::BeginPlay();

	driver = Cast<AScrewDriver>(UGameplayStatics::GetActorOfClass(GetWorld(), AScrewDriver::StaticClass()));
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AScrew::AttachtoDriver);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AScrew::DettachFromDriver);

	startLoc = GetActorLocation();
}

void AScrew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//나사가 드라이버에 붙어있을 때만 회전값이 더해진다
	if(isAttaching == true)
	{
		deltaRot = driver->GetActorRotation().Roll - prevDriverRot;
		rotIntAngle = FMath::RoundHalfFromZero(deltaRot);
		bRotating();
		if(isRotating == true)
		{	
			if (FMath::Abs(deltaRot) > 15) //미세한 회전 방지를 위한 역치 값 
			{
				bEnoughCameOut();
				if(isEnoughRotated == false)
				{
					MoveScrew();
				}
				else
				{
					CameOutScrew();
				}
			}
		}		
	}
}

void AScrew::bRotating()
{	
	if (deltaRot > -120 && deltaRot < 120)
	{
		deltaRot > -15 && deltaRot < 15 ? isRotating = false : isRotating = true;
	}
}

void AScrew::bEnoughCameOut()
{
	//나사가 다 나온 거리인지 확인 : 5 이동하면 다 나온 것
	screwDist = FVector::Dist(GetActorLocation(), startLoc);
	screwDist >= 5 ? isEnoughRotated = true : isEnoughRotated = false;
}

void AScrew::MoveScrew()
{
	if (GetActorLocation().X >= startLoc.X || (deltaRot < -15 && deltaRot > -120))
	{
		//회전
		SetActorRotation(GetActorRotation() + FRotator(0, 0, -1 * rotIntAngle * 0.043f));
		//이동 : rotIntAngle 증가한만큼 나사가 forwardvector로 이동한다
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * rotIntAngle * 0.003f * -1);
	}
	
}

void AScrew::CameOutScrew()
{
	//다 나왔다면 바닥에 떨어질 것
	if(isEnoughRotated == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("CameOutScrew::isEnoughRotated = %d"), isEnoughRotated)
		boxComp->SetSimulatePhysics(true);
	}
}

void AScrew::AttachtoDriver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (driver != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OtherActor = %s"), *OtherActor->GetName())
		isAttaching = true;
		//닿았을 때 최초 회전값
		prevDriverRot = driver->GetActorRotation().Roll;
	}
}

void AScrew::DettachFromDriver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	if (driver != nullptr)
	{ 
		isAttaching = false;	
	}
}