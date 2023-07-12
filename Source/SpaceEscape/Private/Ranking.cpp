// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranking.h"

#include "EscapePlayer.h"
#include "RankingWidget.h"
#include "RoomManager.h"
#include "SpaceEscapeGameModeBase.h"
#include "SpaceEscapeSaveGame.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARanking::ARanking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widgetComp"));
	widgetComp->SetupAttachment(RootComponent);
	widgetComp->SetDrawSize(FVector2D(1080, 1920));
	ConstructorHelpers::FClassFinder<UUserWidget>tempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_RankingWidget.WBP_RankingWidget_C'"));
	if (tempWidget.Succeeded())
	{
		widgetComp->SetWidgetClass(tempWidget.Class);
	}
}

// Called when the game starts or when spawned
void ARanking::BeginPlay()
{
	Super::BeginPlay();
	gm = Cast<ASpaceEscapeGameModeBase>(GetWorld()->GetAuthGameMode());
	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));
	rankingWidget = Cast<URankingWidget>(widgetComp->GetWidget());
	player = Cast<AEscapePlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (rm)
	{
		rm->endingDele.AddUFunction(this, FName("CalculateRecord"));
		rm->endingDele.AddUFunction(this, FName("SetVisible"));
	}

	LoadSavedData();
}

// Called every frame
void ARanking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ���� ���� �� �ű������ �ƴ����� �Ǵ��ϰ� ������ �ٸ��� ����ִ� �Լ�
void ARanking::CalculateRecord()
{
	// ���� ���� �� �ð� ����� �����ͼ�
	curRecord = gm->minute * 60 + gm->second;
	// ����� �����Ϳ� �� �� ��
	rankerMap.GenerateValueArray(rankTimeArr);
	rankTimeArr.Add(curRecord);
	rankTimeArr.Sort([](int a, int b)
	{
			return a < b;
	});
	index = rankTimeArr.Find(curRecord);

	// ��ŷ�� ���� ���ϸ� ��ŷȭ���� ����
	// rankingWidget->nameBlockArray.Num() - 1
	if (index > 8)
	{
		rankingWidget->switcher->SetActiveWidgetIndex(0);
		SetRankingDataToWidget();
	}
	else
	{
		// ��ŷ�� ���ŵȴٸ� �̸� �Է�ȭ���� ����
		if(gm->currentLanguageSetting == ELanguageSettings::KOREAN)
		{
			//�ѱ��� ���
			rankingWidget->switcher->SetActiveWidgetIndex(1);
		}
		else if(gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
		{
			//������ ���
			rankingWidget->switcher->SetActiveWidgetIndex(2);
		}
	}
}

void ARanking::SetVisible()
{
	rankingWidget->SetVisibility(ESlateVisibility::Visible);
}

// SaveGameŬ������ ����� ��ŷ������ �ҷ����� �Լ�
void ARanking::LoadSavedData()
{
	auto loadGameInstance = Cast<USpaceEscapeSaveGame>(UGameplayStatics::CreateSaveGameObject(USpaceEscapeSaveGame::StaticClass()));

	loadGameInstance->saveIndex = 0;
	loadGameInstance->slotName = "SpaceEscapeSaveSlot";

	loadGameInstance = Cast<USpaceEscapeSaveGame>(UGameplayStatics::LoadGameFromSlot(loadGameInstance->slotName, loadGameInstance->saveIndex));
	if (!loadGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("loadGameInstance  is NULL"));
		return;
	}
	rankerMap = loadGameInstance->rankerMap;
}

// ��ŷ������ ��� ���� SaveGameŬ������ �����ϴ� �Լ�
void ARanking::SaveData()
{
	auto saveGameInstance = Cast<USpaceEscapeSaveGame>(UGameplayStatics::CreateSaveGameObject(USpaceEscapeSaveGame::StaticClass()));
	if (!saveGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("saveGameInstance is NULL"));
		return;
	}

	saveGameInstance->saveIndex = 0;
	saveGameInstance->slotName = "SpaceEscapeSaveSlot";

	saveGameInstance->rankerMap = rankerMap;
	UGameplayStatics::SaveGameToSlot(saveGameInstance, saveGameInstance->slotName, saveGameInstance->saveIndex);
}

// ��ŷ�� �ű�� ������ �߰��ϴ� �Լ�
void ARanking::AddToRanking(FString curName)
{
	// �ű���� �ʿ� �߰��Ѵ� (�ϳ��߰�)
	rankerMap.Add(curName, curRecord);

	// ����� �������� sort�Ѵ�
	rankerMap.ValueSort([](int a, int b)
		{
			return a < b;
		});

	// key�� array�� �̾Ƴ���
	rankerMap.GenerateKeyArray(rankNameArr);

	// ����� ������ ��µ����ͺ��� ���ٸ�
	if (rankerMap.Num() > rankingWidget->nameBlockArray.Num())
	{

		// �� ���� key�� �˾Ƴ���
		FString lastRankedName = rankNameArr[rankNameArr.Num() - 1];

		// �� ���� ������ �ʿ��� �����Ѵ� (�ϳ�����)
		rankerMap.Remove(lastRankedName);
	}

	SetRankingDataToWidget();
	SaveData();
}

// ������ ��ŷ�����͸� �Է����ִ� �Լ�
void ARanking::SetRankingDataToWidget()
{
	// key�� value�� array�� �̾Ƴ���
	rankTimeArr.Empty();
	rankNameArr.Empty();
	rankerMap.GenerateValueArray(rankTimeArr);
	rankerMap.GenerateKeyArray(rankNameArr);
	// �ݺ������� �����ϴ� �� �� ��ŭ �������ش�
	for (int i = 0; i < rankingWidget->timeBlockArray.Num(); i++)
	{
		if (rankTimeArr.IsValidIndex(i))
		{
			rankingWidget->timeBlockArray[i]->SetText(FText::FromString(FString::FromInt(rankTimeArr[i] / 60) + " : " + FString::FromInt(rankTimeArr[i] % 60)));
		}
	}

	for (int i = 0; i < rankingWidget->nameBlockArray.Num(); i++)
	{
		if (rankNameArr.IsValidIndex(i))
		{
			rankingWidget->nameBlockArray[i]->SetText(FText::FromString(rankNameArr[i]));
		}
	}
}

