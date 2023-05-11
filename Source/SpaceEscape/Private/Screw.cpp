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
	//���簡 ����̹��� �پ����� ���� ȸ������ ��������
	if(driver->isAttaching == true)
	{
		bRotating();
		if(isRotating == true)
		{	
			if (FMath::Abs(driver->deltaRot) > 15) //�̼��� ȸ�� ������ ���� ��ġ �� 
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
	//���簡 �� ���� �Ÿ����� Ȯ�� : 20 �̵��ϸ� �� ���� ��
	auto screwDist = FVector::Dist(GetActorLocation(), startLoc);
	screwDist >= 20 ? isEnoughRotated = true : isEnoughRotated = false;
}

void AScrew::MoveScrew()
{
	//ȸ��
	SetActorRotation(GetActorRotation() + FRotator(0, 0, -1 * rotIntAngle * 0.03));
	//�̵� : deltaRot �����Ѹ�ŭ ���簡 forwardvector�� �̵��Ѵ�
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * rotIntAngle * 0.003);
}

void AScrew::NoMoveBwd()
{
}

void AScrew::CameOutScrew()
{
	//�� ���Դٸ� �ٴڿ� ������ ��
	if(isEnoughRotated == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameOutScrew::isEnoughRotated = %d"), isEnoughRotated)
		boxComp->SetSimulatePhysics(true);
	}
}
