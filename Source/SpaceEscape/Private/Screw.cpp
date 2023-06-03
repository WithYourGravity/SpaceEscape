// Fill out your copyright notice in the Description page of Project Settings.


#include "Screw.h"
#include "ScrewDriver.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "GrabComponent.h"
#include "MovableCover.h"

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

	Tags.Add(FName("Sense"));
	meshComp->ComponentTags.Add(FName("Sense.R1"));
}

void AScrew::BeginPlay()
{
	Super::BeginPlay();

	bAllFallenScrews = false;

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
	//���簡 �� ���� �Ÿ����� Ȯ�� : 4.6 �̵��ϸ� �� ���� ��
	screwDist = FVector::Dist(GetActorLocation(), startLoc);
	screwDist >= 4.6f ? isEnoughRotated = true : isEnoughRotated = false;
}

void AScrew::MoveScrew()
{
	if (GetActorLocation().X >= startLoc.X || (deltaRot < -15 && deltaRot > -120))
	{
		//ȸ��
		SetActorRotation(GetActorRotation() + FRotator(0, 0, -1 * rotIntAngle * 0.05f));
		//�̵� : rotIntAngle �����Ѹ�ŭ ���簡 forwardvector�� �̵��Ѵ�
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * rotIntAngle * 0.003f * -1);
	}
}

void AScrew::CameOutScrew()
{
	//�� ���Դٸ� �ٴڿ� ������ ��
	//cameOutScrewCount = 0;
	//�� ���Դٸ� �ٴڿ� ������ ��
	if (isEnoughRotated == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("CameOutScrew::isEnoughRotated = %d"), isEnoughRotated)
		boxComp->SetSimulatePhysics(true);
		//cameOutScrewCount++;
	}
	//UE_LOG(LogTemp, Warning, TEXT("cameOutScrew Count : %d"), cameOutScrewCount)

}

void AScrew::CheckAllCameOut()
{
	
	AMovableCover* mc = Cast<AMovableCover>(UGameplayStatics::GetActorOfClass(GetWorld(), AMovableCover::StaticClass()));
	mc->FallingCover();
	UE_LOG(LogTemp, Warning, TEXT("bAllFallenScrews : %d"), bAllFallenScrews)
	cameOutScrewCount = 0;

	//���� 4�� �� Ǯ���� �� Vent ��������� �˷��ٰ�
	//if (cameOutScrewCount > 4)
	//{
		//bool�� cover�� �������� �Ǵ� �������� �˷��� ��
		//UE_LOG(LogTemp, Warning, TEXT("Is cameOutScrew Counted? : %d"), cameOutScrewCount)
	//}
}

void AScrew::AttachtoDriver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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
	if (driver != nullptr)
	{ 
		isAttaching = false;	
	}
}