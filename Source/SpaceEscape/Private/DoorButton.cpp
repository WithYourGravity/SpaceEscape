// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorButton.h"
#include "Doors.h"
#include "Kismet/GameplayStatics.h"
#include "EscapePlayer.h"
#include "Components/BoxComponent.h"
#include "RoomManager.h"
#include "EngineUtils.h"

// Sets default values
ADoorButton::ADoorButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	buttonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("buttonMesh"));
	buttonMesh->SetupAttachment(RootComponent);
	buttonMesh->SetCollisionProfileName(TEXT("PuzzleObjectPreset"));

	buttonMesh->SetVectorParameterValueOnMaterials(FName("doorStateColor"), FVector4(0.505f, 0.015f, 0.00974f, 1));
}

// Called when the game starts or when spawned
void ADoorButton::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoors::StaticClass(),outDoorActors);
	
	ARoomManager* rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoomManager::StaticClass()));
	rm->stageClearDele.AddUFunction(this, FName("CheckClearStage"));
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorButton::OnHandOverlap);
	//UE_LOG(LogTemp, Warning, TEXT("DoorButton::Is Opening State ? : %d"), bOpened)

	/*�� ã��, ��ư�� iterator�� ã������*/
	UGameplayStatics::GetAllActorsOfClass(this, ADoors::StaticClass(), doors);
}

// Called every frame
void ADoorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ADoorButton::CheckClearStage()
{
	bCanButtonClicked = true;
	//stage clear �Ǹ� �� ��ư�� ���� �ٲ�� (Blue)
	for(TActorIterator<AActor> btn(GetWorld()); btn; ++btn)
	{
		buttonMesh->SetVectorParameterValueOnMaterials(FName("doorStateColor"), FVector4(0, 0.573f, 0.49f, 1));
	}
}

void ADoorButton::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Open�Ѵٸ� �÷��̾�� ��Ҵ°�
	AEscapePlayer* player = Cast<AEscapePlayer>(OtherActor);
	if(!player) return;
	//UE_LOG(LogTemp, Warning, TEXT("ADoorButton::OnHandOverlap : %s"), *OtherActor->GetName())
	
	if(bCanButtonClicked == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("OpenAllDoors : bOpened firstly false?: %d"), bOpened)
		if (OtherComp->GetName().Contains("right"))
		{
			//ReportOpen();
			bOpenDoor();
			player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
		}	
		else
		{
			//ReportOpen();
			bOpenDoor();
			player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
		}
	}	
}

void ADoorButton::bOpenDoor()
{
	bOpened == false ? OpenAllDoors() : CloseAllDoors();
}

void ADoorButton::OpenAllDoors()
{
	if(bOpened == false)
	{
		for (AActor* door : doors)
		{
			auto d = Cast<ADoors>(door);
			d->Open();
			
		}
		for (TActorIterator<AActor> it(GetWorld()); it; ++it)
		{
			buttonMesh->SetVectorParameterValueOnMaterials(FName("doorStateColor"), FVector4(0, 0.573f, 0.49f, 1));
			//bOpened = true;
		}
		UE_LOG(LogTemp, Warning, TEXT("OpenAllDoors : bOpened : %d"), bOpened)
	}
	bOpened = true;
}

void ADoorButton::CloseAllDoors()
{
	UE_LOG(LogTemp, Warning, TEXT("CloseAllDoors : bOpened became true?: %d"), bOpened)
	if (bOpened == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("CloseAllDoors : bOpened sibal: %d"), bOpened)
		
		for (AActor* door : doors)
		{
			UE_LOG(LogTemp, Warning, TEXT("CloseAllDoors : iterate sibalsaeKKi: %d"), bOpened)
			auto d = Cast<ADoors>(door);
			d->Close();
		}
		for (TActorIterator<AActor> it(GetWorld()); it; ++it)
		{
			buttonMesh->SetVectorParameterValueOnMaterials(FName("doorStateColor"), FVector4(0.505f, 0.015f, 0.00974f, 1));
			//bOpened = false;
		}
	}
	bOpened = false;
	bCanButtonClicked = false;
	UE_LOG(LogTemp, Warning, TEXT("OpenAllDoors : bOpened changed false?: %d"), bOpened)
}
/*
void ADoorButton::ReportOpen()
{
	openDoorDele.Broadcast();
	
	//������ ��� ��ư�� �����غ���	
	if(bOpened == false)//�ѹ� ���ȴ�.
	{
		bOpened = true;
		//UE_LOG(LogTemp, Warning, TEXT("ReportOpen() : Opened Onced"))
	}
	else
	{
		bOpened = false;
		bCanButtonClicked = false;
		//UE_LOG(LogTemp, Warning, TEXT("ReportOpen() : Not to be Opened"))
		//for (TActorIterator<AActor> it(GetWorld()); it; ++it)
		//{
		//}
	}

}
*/
