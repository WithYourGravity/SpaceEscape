// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"
#include "EngineUtils.h"
#include "EscapePlayer.h"
#include "PuzzleBase.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoomManager::ARoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARoomManager::BeginPlay()
{
	Super::BeginPlay();

	// 퍼즐이 클리어 되면 델리게이트 바인딩된 함수 실행
	for(TActorIterator<APuzzleBase> it(GetWorld()); it; ++it)
	{
		APuzzleBase* pz = *it;
		pz->puzzleClearDele.AddUFunction(this, FName("AddSolvedPuzzleCount"));
	}

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called every frame
void ARoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 완료한 퍼즐 개수 올리고 다 풀었다면 다음 스테이지로 넘어가는 함수
void ARoomManager::AddSolvedPuzzleCount()
{
	solvedPuzzleCount++;

	if (solvedPuzzleCount == totalPuzzlePerStage[playingStage])
	{
		MoveOnNextStage();
	}
	UE_LOG(LogTemp, Warning, TEXT("solved pulzze count is : %d"), solvedPuzzleCount);
	UE_LOG(LogTemp, Warning, TEXT("current stage is : %d"), playingStage);
}

// 다음 스테이지로 넘어가며 퍼즐카운트 초기화 해주는 함수
void ARoomManager::MoveOnNextStage()
{
	playingStage++;
	solvedPuzzleCount = 0;
	if (stageClearDele.IsBound())
	{
		stageClearDele.Broadcast();
	}

	if (gameClearDele.IsBound() && playingStage == 4)
	{
		gameClearDele.Broadcast();
	}
	//UE_LOG(LogTemp, Warning, TEXT("Excuted"));
}

int ARoomManager::GetCurrentPlayingStage()
{
	return playingStage;
}

void ARoomManager::SenseOn()
{
	alpha = 0;
	FTimerHandle senseHandle;
	GetWorldTimerManager().SetTimer(senseHandle, FTimerDelegate::CreateLambda([&]()
		{
			player->vrCamera->SetFieldOfView(FMath::Lerp(90.f, 120.f, alpha));
			alpha += GetWorld()->GetDeltaSeconds();
			if (alpha >= 1)
			{
				GetWorldTimerManager().ClearTimer(senseHandle);
			}
		}), GetWorld()->GetDeltaSeconds(), true);
	player->vrCamera->PostProcessSettings.ColorSaturation = FVector4::Zero();

	HighlightObject();
}

void ARoomManager::SenseOff()
{
	alpha = 0;
	FTimerHandle senseHandle;
	GetWorldTimerManager().SetTimer(senseHandle, FTimerDelegate::CreateLambda([&]()
		{
			player->vrCamera->SetFieldOfView(FMath::Lerp(120.f, 90.f, alpha));
			alpha += GetWorld()->GetDeltaSeconds();
			if (alpha >= 1)
			{
				GetWorldTimerManager().ClearTimer(senseHandle);
			}
		}), GetWorld()->GetDeltaSeconds(), true);
	player->vrCamera->PostProcessSettings.ColorSaturation = FVector4::One();

	UnHilightObject();
}

void ARoomManager::HighlightObject()
{

}

void ARoomManager::UnHilightObject()
{

}

