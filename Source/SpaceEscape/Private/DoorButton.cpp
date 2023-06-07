// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorButton.h"
#include "Doors.h"
#include "Kismet/GameplayStatics.h"
#include "EscapePlayer.h"
#include "Components/BoxComponent.h"
#include "RoomManager.h"
#include "DoorManager.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

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

	ConstructorHelpers::FObjectFinder<USoundCue> btnSound(TEXT("/Script/Engine.SoundWave'/Game/Yeni/Music/SoundEffect/BtnClickSound.BtnClickSound'"));
	if (btnSound.Succeeded())
	{
		btnSoundCue = btnSound.Object;
	}
	btnAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("btnAudioComp"));
	btnAudioComp->SetupAttachment(RootComponent);
	btnAudioComp->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ADoorButton::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoors::StaticClass(),outDoorActors);

	/*Room Manager*/
	ARoomManager* rManager = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoomManager::StaticClass()));
	rManager->stageClearDele.AddUFunction(this, FName("CheckClearStage"));
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorButton::OnHandOverlap);

	/*Door Manager*/
	dManager = Cast<ADoorManager>(UGameplayStatics::GetActorOfClass(this, ADoorManager::StaticClass()));
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

void ADoorButton::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Open한다면 플레이어와 닿았는가
	AEscapePlayer* player = Cast<AEscapePlayer>(OtherActor);
	if(!player) return;
	//UE_LOG(LogTemp, Warning, TEXT("ADoorButton::OnHandOverlap : %s"), *OtherActor->GetName())

	//효과음 재생
	if (btnSoundCue && btnAudioComp->IsValidLowLevelFast())
	{
		btnAudioComp->SetSound(btnSoundCue);
		btnAudioComp->Play();
	}

	if(!dManager) return;
	if(bCanButtonClicked == true)
	{
		if(dManager->bOpened == false)
		{
			dManager->OpenAllDoors();
			if (OtherComp->GetName().Contains("right"))
			{
				player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
			}
			else
			{
				player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
			}
		}
		else
		{
			dManager->CloseAllDoors();
			if (OtherComp->GetName().Contains("right"))
			{
				player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Right);
			}
			else
			{
				player->GetLocalViewingPlayerController()->PlayHapticEffect(hapticFeedback, EControllerHand::Left);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("DoorButton::OnHandOverlap"))
	}	
}

void ADoorButton::ReportOpen()
{
	openDoorDele.Broadcast();
	
	if(bOpened == false)//한번 열렸다.
	{
		bOpened = true;
		//UE_LOG(LogTemp, Warning, TEXT("ReportOpen() : Opened Onced"))
	}
	else
	{
		bOpened = false;
		bCanButtonClicked = false;
	}
}
