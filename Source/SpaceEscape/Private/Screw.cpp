// Fill out your copyright notice in the Description page of Project Settings.


#include "Screw.h"
#include "ScrewDriver.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GrabComponent.h"

AScrew::AScrew()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Screw Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(2, 1, 3));
	boxComp->SetRelativeRotation(FRotator(0, 0, 0));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewMesh"));
	meshComp->SetupAttachment(RootComponent);
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	arrowComp->SetupAttachment(RootComponent);
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComp"));
	grabComp->SetupAttachment(RootComponent);
	grabComp->grabType = EGrabType::FREE;
}

void AScrew::BeginPlay()
{
	Super::BeginPlay();

	driver = Cast<AScrewDriver>(UGameplayStatics::GetActorOfClass(GetWorld(), AScrewDriver::StaticClass()));

	startLoc = GetActorLocation();
}

void AScrew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//나사가 드라이버에 붙어있을 때만 회전값이 더해진다
	if(driver->isAttaching == true)
	{
		bRotating();
		if(isRotating == true)
		{	
			if (FMath::Abs(driver->deltaRot) > 15) //미세한 회전 방지를 위한 역치 값 
			{
				UE_LOG(LogTemp, Warning, TEXT("Tick IsRotating::isEnoughRotated = %d"), isEnoughRotated)
				bEnoughCameOut();
				if(isEnoughRotated == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("Tick::isEnoughRotatedisEnoughRotated = %d"), isEnoughRotated)
					rotIntAngle = FMath::RoundHalfFromZero(driver->deltaRot);
					MoveScrew();
					//coeff =  FMath::Abs(driver->deltaRot) / 7;					
				}
				else
				{
					CameOutScrew();
				}
			}
		}		
	}
	else
	{
		rotIntAngle = 0;
		//coeff = 0;
	}
}

void AScrew::bRotating()
{	
	if (driver->deltaRot > -150 && driver->deltaRot < 150)
	{
		driver->deltaRot > -15 && driver->deltaRot < 15 ? isRotating = false : isRotating = true;
	}
}

void AScrew::bEnoughCameOut()
{
	//나사가 다 나온 거리인지 확인 : 20 이동하면 다 나온 것
	auto screwDist = FVector::Dist(GetActorLocation(), startLoc);
	screwDist >= 20 ? isEnoughRotated = true : isEnoughRotated = false;
}

void AScrew::MoveScrew()
{
	//회전
	SetActorRotation(GetActorRotation() + FRotator(0, 0, -1 * rotIntAngle * 0.03));
	//이동 : deltaRot 증가한만큼 나사가 forwardvector로 이동한다
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * rotIntAngle * 0.003);
}

void AScrew::NoMoveBwd()
{
}

void AScrew::CameOutScrew()
{
	//다 나왔다면 바닥에 떨어질 것
	if(isEnoughRotated == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameOutScrew::isEnoughRotated = %d"), isEnoughRotated)
		boxComp->SetSimulatePhysics(true);
	}
}
