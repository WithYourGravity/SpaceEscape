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
}

// Called every frame
void ADoorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ADoorButton::CheckClearStage()
{
	bCanButtonClicked = true;
}

void ADoorButton::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Open한다면 플레이어와 닿았는가
	AEscapePlayer* player = Cast<AEscapePlayer>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("ADoorButton::OnHandOverlap : %s"), *OtherActor->GetName())
	if(bCanButtonClicked == true)
	{
		if (player && OtherComp->GetName().Contains("right"))
		{
			ReportOpen();
			player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
		}	
		else
		{
			ReportOpen();
			player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
		}
	}	
}

void ADoorButton::ReportOpen()
{	
	openDoorDele.Broadcast();
	
	if(bOpened == false)//한번 열렸다.
	{
		bOpened = true;
		//UE_LOG(LogTemp, Warning, TEXT("ReportOpen() : Opened Onced"))
		//문 열릴 때 색 바뀌기
		buttonMesh->SetVectorParameterValueOnMaterials(FName("doorStateColor"), FVector4(0.505f, 0.015f, 0.00974f, 1));
	}
	else
	{
		bOpened = false;
		bCanButtonClicked = false;
		//UE_LOG(LogTemp, Warning, TEXT("ReportOpen() : Not to be Opened"))
		buttonMesh->SetVectorParameterValueOnMaterials(FName("doorStateColor"), FVector4(0, 0.573f, 0.49f, 1));
	}
}

