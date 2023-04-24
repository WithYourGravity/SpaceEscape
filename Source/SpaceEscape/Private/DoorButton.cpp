// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorButton.h"

#include "Doors.h"
#include "Kismet/GameplayStatics.h"
#include "EscapePlayer.h"
#include "Components/BoxComponent.h"
#include "RoomManager.h"

// Sets default values
ADoorButton::ADoorButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	buttonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("buttonMesh"));
	buttonMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoorButton::BeginPlay()
{
	Super::BeginPlay();
	
	door = Cast<ADoors>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoors::StaticClass()));

	ARoomManager* rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoomManager::StaticClass()));
	rm->stageClearDele.AddUFunction(this, FName("CheckClearStage"));
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorButton::OnHandOverlap);
}

// Called every frame
void ADoorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorButton::CheckClearStage()
{
	UE_LOG(LogTemp, Warning, TEXT("CheckClearStage Function"))
	bCanButtonClicked = true;
}

void ADoorButton::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Open한다면 플레이어와 닿았는가
	UE_LOG(LogTemp, Warning, TEXT("ADoorButton::OnHandOverlap"))
	AEscapePlayer* player = Cast<AEscapePlayer>(OtherActor);
	if(bCanButtonClicked == true)
	{
		if (player != nullptr)
		{
			ReportOpen();
		}
	}	
}

void ADoorButton::ReportOpen()
{	
	UE_LOG(LogTemp, Warning, TEXT("ReportOpen!!"))	
	openDoorDele.Execute();
	
	if(bOpened == false)//한번 열렸다.
	{
		bOpened = true;
	}
	else
	{
		bOpened = false;
		bCanButtonClicked = false;
	}
}

