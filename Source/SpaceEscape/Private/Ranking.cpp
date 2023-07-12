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

// 게임 종료 후 신기록인지 아닌지를 판단하고 위젯을 다르게 띄워주는 함수
void ARanking::CalculateRecord()
{
	// 게임 종료 후 시간 기록을 가져와서
	curRecord = gm->minute * 60 + gm->second;
	// 저장된 데이터와 비교 한 후
	rankerMap.GenerateValueArray(rankTimeArr);
	rankTimeArr.Add(curRecord);
	rankTimeArr.Sort([](int a, int b)
	{
			return a < b;
	});
	index = rankTimeArr.Find(curRecord);

	// 랭킹에 들지 못하면 랭킹화면을 띄우고
	// rankingWidget->nameBlockArray.Num() - 1
	if (index > 8)
	{
		rankingWidget->switcher->SetActiveWidgetIndex(0);
		SetRankingDataToWidget();
	}
	else
	{
		// 랭킹이 갱신된다면 이름 입력화면을 띄운다
		if(gm->currentLanguageSetting == ELanguageSettings::KOREAN)
		{
			//한글일 경우
			rankingWidget->switcher->SetActiveWidgetIndex(1);
		}
		else if(gm->currentLanguageSetting == ELanguageSettings::ENGLISH)
		{
			//영어일 경우
			rankingWidget->switcher->SetActiveWidgetIndex(2);
		}
	}
}

void ARanking::SetVisible()
{
	rankingWidget->SetVisibility(ESlateVisibility::Visible);
}

// SaveGame클래스에 저장된 랭킹정보를 불러오는 함수
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

// 랭킹정보가 담긴 맵을 SaveGame클래스로 저장하는 함수
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

// 랭킹에 신기록 정보를 추가하는 함수
void ARanking::AddToRanking(FString curName)
{
	// 신기록을 맵에 추가한다 (하나추가)
	rankerMap.Add(curName, curRecord);

	// 기록을 기준으로 sort한다
	rankerMap.ValueSort([](int a, int b)
		{
			return a < b;
		});

	// key를 array로 뽑아낸다
	rankerMap.GenerateKeyArray(rankNameArr);

	// 기록이 위젯의 출력데이터보다 많다면
	if (rankerMap.Num() > rankingWidget->nameBlockArray.Num())
	{

		// 맨 꼴찌 key를 알아낸다
		FString lastRankedName = rankNameArr[rankNameArr.Num() - 1];

		// 맨 꼴찌 정보를 맵에서 삭제한다 (하나삭제)
		rankerMap.Remove(lastRankedName);
	}

	SetRankingDataToWidget();
	SaveData();
}

// 위젯에 랭킹데이터를 입력해주는 함수
void ARanking::SetRankingDataToWidget()
{
	// key와 value를 array로 뽑아낸다
	rankTimeArr.Empty();
	rankNameArr.Empty();
	rankerMap.GenerateValueArray(rankTimeArr);
	rankerMap.GenerateKeyArray(rankNameArr);
	// 반복문으로 존재하는 블럭 수 만큼 세팅해준다
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

