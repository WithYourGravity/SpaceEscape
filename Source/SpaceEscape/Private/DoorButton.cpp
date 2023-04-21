// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorButton.h"

#include "EscapePlayer.h"
//#include "Doors.h"
#include "Components/BoxComponent.h"
//#include "Kismet/GameplayStatics.h"
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
}

// Called when the game starts or when spawned
void ADoorButton::BeginPlay()
{
	Super::BeginPlay();
	
	//door = Cast<ADoors>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoors::StaticClass()));
	
	for (TActorIterator<ARoomManager> it(GetWorld()); it; ++it)
	{
		ARoomManager* rm = *it;
		rm->stageClearDele.AddUFunction(this, FName("CheckClearStage"));

	}
	
}

// Called every frame
void ADoorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorButton::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//플레이어와 닿으면 퍼즐 해결여부 체크
	player = Cast<AEscapePlayer>(OtherActor);
	player != nullptr ? ReportOpen() : NoReportOpen();
}

void ADoorButton::CheckClearStage()
{
	//문 열것인가 안열것인가
	//문 열거면 플레이어와 닿았는가
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorButton::OnHandOverlap);
	//bIsOpen == true ? ReportOpen() : NoReportOpen();
	UE_LOG(LogTemp, Warning, TEXT("CheckClearStage"))
}

void ADoorButton::ReportOpen()
{	
	UE_LOG(LogTemp, Warning, TEXT("CheckClearStage : Puzzle Clear, Character and DoorButton"))
	openDoorDele.Execute();
}

void ADoorButton::NoReportOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("NoReportOpen"))
	return;
}


