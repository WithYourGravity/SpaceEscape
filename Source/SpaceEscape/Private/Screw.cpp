// Fill out your copyright notice in the Description page of Project Settings.


#include "Screw.h"
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
	//���簡 ����̹��� �پ����� ���� ȸ������ ��������
	if(isAttaching == true)
	{
		deltaRot = driver->GetActorRotation().Roll - prevDriverRot;
		rotIntAngle = FMath::RoundHalfFromZero(deltaRot);
		bRotating();
		if(isRotating == true)
		{	
			if (FMath::Abs(deltaRot) > 15) //�̼��� ȸ�� ������ ���� ��ġ �� 
			{
				//UE_LOG(LogTemp, Warning, TEXT("Tick IsRotating::isEnoughRotated = %d"), isEnoughRotated)
				bEnoughCameOut();
				if(isEnoughRotated == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("Tick::isEnoughRotatedisEnoughRotated = %d"), isEnoughRotated)
					//rotIntAngle = FMath::RoundHalfFromZero(deltaRot);
					UE_LOG(LogTemp, Warning, TEXT("Tick::MoveScrew() driver->deltaRot :: %f"), deltaRot)
					UE_LOG(LogTemp, Warning, TEXT("Tick::MoveScrew() rotAngle :: %d"), rotIntAngle)
					MoveScrew();
					UE_LOG(LogTemp, Warning, TEXT("Tick::ScrewDist :: %lf"), screwDist)
				}
				else
				{
					CameOutScrew();
					UE_LOG(LogTemp, Warning, TEXT("Tick::ScrewDist :: %lf"), screwDist)
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
	//���簡 �� ���� �Ÿ����� Ȯ�� : 5 �̵��ϸ� �� ���� ��
	screwDist = FVector::Dist(GetActorLocation(), startLoc);
	screwDist >= 5 ? isEnoughRotated = true : isEnoughRotated = false;
}

void AScrew::MoveScrew()
{
	if (GetActorLocation().X >= startLoc.X || (deltaRot < -15 && deltaRot > -120))
	{
		//ȸ��
		SetActorRotation(GetActorRotation() + FRotator(0, 0, -1 * rotIntAngle * 0.043f));
		//�̵� : rotIntAngle �����Ѹ�ŭ ���簡 forwardvector�� �̵��Ѵ�
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * rotIntAngle * 0.003f * -1);
		UE_LOG(LogTemp, Warning, TEXT("MoveScrew() rotIntAngle :: %d"), rotIntAngle)
		UE_LOG(LogTemp, Warning, TEXT("MoveScrew() startLoc.X :: %f"), startLoc.X)
		UE_LOG(LogTemp, Warning, TEXT("MoveScrew() current Location.X :: %f"), GetActorLocation().X)
	}
	
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

void AScrew::AttachtoDriver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//AScrewDriver* attachedDriver = Cast<AScrewDriver>(OtherActor);
	if (driver != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OtherActor = %s"), *OtherActor->GetName())
		isAttaching = true;
		//����� �� ���� ȸ����
		prevDriverRot = driver->GetActorRotation().Roll;
	}
}

void AScrew::DettachFromDriver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	//AScrewDriver* attachedDriver = Cast<AScrewDriver>(OtherActor);
	if (driver != nullptr)
	{ 
		isAttaching = false;	
	}
}