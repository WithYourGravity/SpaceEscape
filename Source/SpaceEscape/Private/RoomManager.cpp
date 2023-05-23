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

	// ������ Ŭ���� �Ǹ� ��������Ʈ ���ε��� �Լ� ����
	for(TActorIterator<APuzzleBase> it(GetWorld()); it; ++it)
	{
		APuzzleBase* pz = *it;
		pz->puzzleClearDele.AddUFunction(this, FName("AddSolvedPuzzleCount"));
	}

	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));

	GetInteractionObjectToArray();
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
	UE_LOG(LogTemp, Warning, TEXT("current stage is : %d"), playingStage);
}

// ���� ���������� �Ѿ�� ����ī��Ʈ �ʱ�ȭ ���ִ� �Լ�
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
	player->vrCamera->PostProcessSettings.ColorSaturation = FVector4::Zero();

	for(UStaticMeshComponent* sm : arrSenseAlwaysComp)
	{
		sm->SetRenderCustomDepth(true);
		sm->CustomDepthStencilValue = 1;
	}

	if (playingStage == 1)
	{
		for (UStaticMeshComponent* sm : arrSenseR1Comp)
		{
			sm->SetRenderCustomDepth(true);
			sm->CustomDepthStencilValue = 1;
		}
	}
	else if (playingStage == 2)
	{
		for (UStaticMeshComponent* sm : arrSenseR2Comp)
		{
			sm->SetRenderCustomDepth(true);
			sm->CustomDepthStencilValue = 1;
		}
	}
	else if (playingStage == 3)
	{
		for (UStaticMeshComponent* sm : arrSenseR3Comp)
		{
			sm->SetRenderCustomDepth(true);
			sm->CustomDepthStencilValue = 1;
		}
	}
}

void ARoomManager::SenseOff()
{
	player->vrCamera->PostProcessSettings.ColorSaturation = FVector4::One();

	for (UStaticMeshComponent* sm : arrSenseAlwaysComp)
	{
		sm->SetRenderCustomDepth(false);
		sm->CustomDepthStencilValue = 0;
	}

	if (playingStage == 1)
	{
		for (UStaticMeshComponent* sm : arrSenseR1Comp)
		{
			sm->SetRenderCustomDepth(false);
			sm->CustomDepthStencilValue = 0;
		}
	}
	else if (playingStage == 2)
	{
		for (UStaticMeshComponent* sm : arrSenseR2Comp)
		{
			sm->SetRenderCustomDepth(false);
			sm->CustomDepthStencilValue = 0;
		}
	}
	else if (playingStage == 3)
	{
		for (UStaticMeshComponent* sm : arrSenseR3Comp)
		{
			sm->SetRenderCustomDepth(false);
			sm->CustomDepthStencilValue = 0;
		}
	}
}

void ARoomManager::GetInteractionObjectToArray()
{
	// ���忡 �ִ� ��� ���͸� �˻��Ѵ�
	for (TActorIterator<AActor> it(GetWorld()); it; ++it)
	{
		AActor* actor = *it;
		// sense�±װ� �پ��ִٸ�
		if (actor->ActorHasTag(FName("Sense")))
		{
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
}
