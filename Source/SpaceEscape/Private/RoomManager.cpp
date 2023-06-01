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
	ConstructorHelpers::FObjectFinder<ULevelSequence>tempSeq(TEXT("/Script/LevelSequence.LevelSequence'/Game/Yeni/LevelSequences/Seq_AfterShipRotated.Seq_AfterShipRotated'"));
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

	// ������ Ŭ���� �Ǹ� ��������Ʈ ���ε��� �Լ� ����
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

// �Ϸ��� ���� ���� �ø��� �� Ǯ���ٸ� ���� ���������� �Ѿ�� �Լ�
void ARoomManager::AddSolvedPuzzleCount()
{
	solvedPuzzleCount++;

	if (solvedPuzzleCount == totalPuzzlePerStage[playingStage])
	{
		MoveOnNextStage();
	}
	UE_LOG(LogTemp, Warning, TEXT("solved pulzze count is : %d"), solvedPuzzleCount);
	UE_LOG(LogTemp, Warning, TEXT("current stage is : %f"), playingStage);
}

// ���� ���������� �Ѿ�� ����ī��Ʈ �ʱ�ȭ ���ִ� �Լ�
void ARoomManager::MoveOnNextStage()
{
	SenseOff();
	playingStage += 0.5f;
	solvedPuzzleCount = 0;

	if (stageClearDele.IsBound())
	{
		stageClearDele.Broadcast();
	}

	if (gameClearDele.IsBound() && playingStage == 3.5)
	{
		gameClearDele.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("gameClearDele Excuted"));
	}

	if (spawnEnemyDele.IsBound() && (playingStage == 1 || playingStage == 2.5))
	{
		spawnEnemyDele.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("spawnEnemyDele Excuted"));
	}

	UE_LOG(LogTemp, Warning, TEXT("solved pulzze count is : %d"), solvedPuzzleCount);
	UE_LOG(LogTemp, Warning, TEXT("current stage is : %f"), playingStage);
}

void ARoomManager::StageProgressChecker()
{
	playingStage += 0.5f;

	// ���� ó�� �ʿ�
	if (playingStage == 4)
	{
		// �÷���Ÿ�� ����
		gm->StopPlayTime();

		// �÷��̾� �� �ݸ��� ���� ������ �� ����
		player->leftIndexFingerCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		player->rightIndexFingerCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		player->UnTryGrabLeft();
		player->UnTryGrabRight();

		// ������ ���
		sequencePlayer->Play();
	}

	UE_LOG(LogTemp, Warning, TEXT("ARoomManager::StageProgressChecker"));
	UE_LOG(LogTemp, Warning, TEXT("current stage is : %f"), playingStage);
}

void ARoomManager::SequenceFinished()
{
	// ���ּ� ���ֹ����� �̵�
	ship->SetActorLocation(FVector(-3300, 18000, 1000));
	ship->SetActorRotation(FRotator(0, 90, 0));
	player->SetActorLocation(ship->forLocComp->GetComponentLocation());

	// ��ŷ ���� �ѱ�
	endingDele.Broadcast();

	// �÷��̾� �� �ݸ��� ����
	player->leftIndexFingerCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	player->rightIndexFingerCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	player->ActiveRightWidgetInteraction();
}

// ���� stage�� ��ȯ�ϴ� �Լ�
float ARoomManager::GetCurrentPlayingStage()
{
	return playingStage;
}

int ARoomManager::GetSolvedPuzzleCount()
{
	return solvedPuzzleCount;
}

// ���� ��� ���ִ� �Լ�
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

// ���� ��� ���� �Լ�
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

// ���� ���� �� ��� ���͸� �˻��� ������ ǥ���� ���͸� ĳ���س��´�
void ARoomManager::GetInteractionObjectToArray()
{
	// ���忡 �ִ� ��� ���͸� �˻��Ѵ�
	for (TActorIterator<AActor> it(GetWorld()); it; ++it)
	{
		AActor* actor = *it;
		// sense�±װ� �پ��ִٸ�
		if (!actor->ActorHasTag(FName("Sense")))
		{
			continue;
		}
		// ������Ʈ�� �±׸� Ȯ���Ͽ� �з��Ѵ�
		auto compArray = actor->GetComponentsByTag(smComp, FName("Sense.R1"));
		// �±װ� Ȯ�εǸ�
		if (!compArray.IsEmpty())
		{
			for (UActorComponent* comp : compArray)
			{
				// StaticMesh�� ĳ�����ؼ� array�� �߰��س��´�
				UStaticMeshComponent* sm = Cast<UStaticMeshComponent>(comp);
				arrSenseR1Comp.Add(sm);
			}
		}
		// ���� �±� Ȯ�� ���� array�� ����
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
		compArray.Empty();
	}
}
