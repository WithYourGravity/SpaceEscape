// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"
#include "EngineUtils.h"
#include "EscapePlayer.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "PuzzleBase.h"
#include "SpaceEscapeGameModeBase.h"
#include "SpaceShip.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoomManager::ARoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	/* 
	const ConstructorHelpers::FObjectFinder<ULevelSequence>tempSeq(TEXT("/Script/LevelSequence.LevelSequence'/Game/Yeni/LevelSequences/Seq_AfterShipRotated.Seq_AfterShipRotated'"));
    if (tempSeq.Succeeded())
    {
		sequenceAsset = tempSeq.Object;
    }
    */
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
	ship = Cast<ASpaceShip>(UGameplayStatics::GetActorOfClass(this, ASpaceShip::StaticClass()));
	gm = Cast<ASpaceEscapeGameModeBase>(GetWorld()->GetAuthGameMode());

	// sense
	GetInteractionObjectToArray();

	// sequence
	FMovieSceneSequencePlaybackSettings seqSetting;
	seqSetting.bDisableMovementInput = true;
	sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this, sequenceAsset, seqSetting, sequenceActor);
	sequencePlayer->OnFinished.AddDynamic(this, &ARoomManager::SequenceFinished);
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

	if (!totalPuzzlePerStage.Contains(playingStage))
	{
		UE_LOG(LogTemp, Warning, TEXT("ARoomManager::AddSolvedPuzzleCount : Current Stage : %f"), playingStage);
		UE_LOG(LogTemp, Error, TEXT("ARoomManager::AddSolvedPuzzleCount : Invalid Stage!!"));
		return;
	}

	if (solvedPuzzleCount == totalPuzzlePerStage[playingStage])
	{
		MoveOnNextStage();
		UE_LOG(LogTemp, Warning, TEXT("ARoomManager::AddSolvedPuzzleCount : MoveOnNextStage"));
	}
}

// 다음 스테이지로 넘어가며 퍼즐카운트 초기화 해주는 함수
void ARoomManager::MoveOnNextStage()
{
	SenseOff();
	playingStage += 0.5f;
	solvedPuzzleCount = 0;

	if (stageClearDele.IsBound() && (playingStage == 1.5 || playingStage == 2.5))
	{
		stageClearDele.Broadcast();
	}

	if (gameClearDele.IsBound() && playingStage == 3.5)
	{
		gameClearDele.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("gameClearDele Broadcasted"));
	}

	if (spawnEnemyDele.IsBound() && (playingStage == 1 || playingStage == 2.5))
	{
		spawnEnemyDele.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("spawnEnemyDele Broadcasted"));
	}

	// 플레이어 독백 위한 델리게이트
	if (forStoryDele.IsBound() && (playingStage == 2 || playingStage == 3))
	{
		forStoryDele.Broadcast();
	}

	UE_LOG(LogTemp, Warning, TEXT("ARoomManager::MoveOnNextStage : Current Stage : %f"), playingStage);
}

void ARoomManager::StageProgressChecker()
{
	playingStage += 0.5f;

	// 플레이어 독백 위한 델리게이트
	if (forStoryDele.IsBound() && (playingStage == 2 || playingStage == 3))
	{
		forStoryDele.Broadcast();
	}

	// 엔딩 처리 필요
	if (playingStage == 4)
	{
		// 플레이타임 정지
		gm->StopPlayTime();

		// 플레이어 손 콜리전 끄고 잡은거 다 놓게
		player->leftIndexFingerCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		player->rightIndexFingerCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		player->UnTryGrabLeft();
		player->UnTryGrabRight();

		// 시퀀스 재생
		sequencePlayer->Play();
	}

	UE_LOG(LogTemp, Warning, TEXT("ARoomManager::StageProgressChecker : current stage is : %f"), playingStage);
}

void ARoomManager::SequenceFinished()
{
	// 우주선 우주밖으로 이동
	ship->SetActorLocation(FVector(-3366.f, 16114.44f, 983.f));
	ship->SetActorRotation(FRotator(0, 89.f, 0));
	player->SetActorLocation(ship->forLocComp->GetComponentLocation());

	// 랭킹 위젯 켜기
	endingDele.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("ARoomManager::SequenceFinished : endingDele Broadcasted"));

	// 플레이어 손 콜리전 복구
	player->leftIndexFingerCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	player->rightIndexFingerCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	player->ActiveRightWidgetInteraction();
}

// 현재 stage를 반환하는 함수
float ARoomManager::GetCurrentPlayingStage()
{
	return playingStage;
}

// 1번방 배터리 장착되면 터치패드 동작하게 할 용도로 만든 함수
int ARoomManager::GetSolvedPuzzleCount()
{
	return solvedPuzzleCount;
}

// 센스 기능 켜주는 함수
void ARoomManager::SenseOn()
{
	player->vrCamera->PostProcessSettings.ColorSaturation = FVector4::Zero();

	for(UStaticMeshComponent* sm : arrSenseAlwaysComp)
	{
		sm->SetRenderCustomDepth(true);
		sm->CustomDepthStencilValue = 1;
	}

	switch ((int)playingStage)
	{
	case 1 :
		for (UStaticMeshComponent* sm : arrSenseR1Comp)
		{
			sm->SetRenderCustomDepth(true);
			sm->CustomDepthStencilValue = 1;
		}
		break;
	case 2 :
		for (UStaticMeshComponent* sm : arrSenseR2Comp)
		{
			sm->SetRenderCustomDepth(true);
			sm->CustomDepthStencilValue = 1;
		}
		break;
	case 3:
		for (UStaticMeshComponent* sm : arrSenseR3Comp)
		{
			sm->SetRenderCustomDepth(true);
			sm->CustomDepthStencilValue = 1;
		}
		break;
	case 4:
		for (UStaticMeshComponent* sm : arrSenseR4Comp)
		{
			sm->SetRenderCustomDepth(true);
			sm->CustomDepthStencilValue = 1;
		}
		break;
	}
}

// 센스 기능 끄는 함수
void ARoomManager::SenseOff()
{
	player->vrCamera->PostProcessSettings.ColorSaturation = FVector4::One();

	for (UStaticMeshComponent* sm : arrSenseAlwaysComp)
	{
		sm->SetRenderCustomDepth(false);
		sm->CustomDepthStencilValue = 0;
	}

	switch ((int)playingStage)
	{
	case 1:
		for (UStaticMeshComponent* sm : arrSenseR1Comp)
		{
			sm->SetRenderCustomDepth(false);
			sm->CustomDepthStencilValue = 0;
		}
		break;
	case 2:
		for (UStaticMeshComponent* sm : arrSenseR2Comp)
		{
			sm->SetRenderCustomDepth(false);
			sm->CustomDepthStencilValue = 0;
		}
		break;
	case 3:
		for (UStaticMeshComponent* sm : arrSenseR3Comp)
		{
			sm->SetRenderCustomDepth(false);
			sm->CustomDepthStencilValue = 0;
		}
		break;
	case 4:
		for (UStaticMeshComponent* sm : arrSenseR4Comp)
		{
			sm->SetRenderCustomDepth(false);
			sm->CustomDepthStencilValue = 0;
		}
		break;
	}
}

// 게임 시작 시 모든 액터를 검사해 센스에 표시할 액터를 캐싱해놓는다
void ARoomManager::GetInteractionObjectToArray()
{
	// 월드에 있는 모든 액터를 검사한다
	for (TActorIterator<AActor> it(GetWorld()); it; ++it)
	{
		AActor* actor = *it;
		// sense태그가 붙어있다면
		if (!actor->ActorHasTag(FName("Sense")))
		{
			continue;
		}
		// 컴포넌트에 태그를 확인하여 분류한다
		auto compArray = actor->GetComponentsByTag(smComp, FName("Sense.R1"));
		// 태그가 확인되면
		if (!compArray.IsEmpty())
		{
			for (UActorComponent* comp : compArray)
			{
				// StaticMesh로 캐스팅해서 array에 추가해놓는다
				UStaticMeshComponent* sm = Cast<UStaticMeshComponent>(comp);
				arrSenseR1Comp.Add(sm);
			}
		}
		// 다음 태그 확인 위해 array를 비운다
		compArray.Empty();

		compArray = actor->GetComponentsByTag(smComp, FName("Sense.R2"));
		if (!compArray.IsEmpty())
		{
			for (UActorComponent* comp : compArray)
			{
				UStaticMeshComponent* sm = Cast<UStaticMeshComponent>(comp);
				arrSenseR2Comp.Add(sm);
			}
		}
		compArray.Empty();

		compArray = actor->GetComponentsByTag(smComp, FName("Sense.R3"));
		if (!compArray.IsEmpty())
		{
			for (UActorComponent* comp : compArray)
			{
				UStaticMeshComponent* sm = Cast<UStaticMeshComponent>(comp);
				arrSenseR3Comp.Add(sm);
			}
		}
		compArray.Empty();

		compArray = actor->GetComponentsByTag(smComp, FName("Sense.R4"));
		if (!compArray.IsEmpty())
		{
			for (UActorComponent* comp : compArray)
			{
				UStaticMeshComponent* sm = Cast<UStaticMeshComponent>(comp);
				arrSenseR4Comp.Add(sm);
			}
		}
		compArray.Empty();

		compArray = actor->GetComponentsByTag(smComp, FName("Sense.Always"));
		if (!compArray.IsEmpty())
		{
			for (UActorComponent* comp : compArray)
			{
				UStaticMeshComponent* sm = Cast<UStaticMeshComponent>(comp);
				arrSenseAlwaysComp.Add(sm);
			}
		}
	}
}
