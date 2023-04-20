// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorButton.h"

#include "EscapePlayer.h"
#include "Doors.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorButton::OnHandOverlap);

}

// Called every frame
void ADoorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorButton::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//문이 열린다
	player = Cast<AEscapePlayer>(OtherActor);
	if(player->rightHand)
	{
		UE_LOG(LogTemp, Warning, TEXT("On Hand Overlap : Character and DoorButton"))
		openDoorDele.Execute();		
	}
}


