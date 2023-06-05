// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorManager.h"
#include "Doors.h"
#include "Kismet/GameplayStatics.h"
#include "DoorButton.h"
#include "RoomManager.h"

// Sets default values
ADoorManager::ADoorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoorManager::BeginPlay()
{
	Super::BeginPlay();

	//문, 버튼 찾기
	UGameplayStatics::GetAllActorsOfClass(this, ADoors::StaticClass(), doors);
	UGameplayStatics::GetAllActorsOfClass(this, ADoorButton::StaticClass(), doorBtns);
	rm = Cast<ARoomManager>(UGameplayStatics::GetActorOfClass(this, ARoomManager::StaticClass()));

}

// Called every frame
void ADoorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorManager::OpenAllDoors()
{
	if (rm->GetCurrentPlayingStage() - (int)rm->GetCurrentPlayingStage() == 0)
	{
		return;
	}
	for(AActor* dbtn : doorBtns)
	{
		auto db = Cast<ADoorButton>(dbtn);
		//파란불
		db->buttonMesh->SetVectorParameterValueOnMaterials(FName("doorStateColor"), FVector4(0, 0.573f, 0.49f, 1));
		//db->buttonMesh->SetVectorParameterValueOnMaterials(FName("doorStateColor"), FVector4(0.505f, 0.015f, 0.00974f, 1));
	}
	for (AActor* door : doors)
	{
		auto d = Cast<ADoors>(door);
		d->Open();
	}
	bOpened = true;
	UE_LOG(LogTemp, Warning, TEXT("DoorManager::OpenAllDoors"))
}

void ADoorManager::CloseAllDoors()
{
	
	for (AActor* door : doors)
	{
		UE_LOG(LogTemp, Warning, TEXT("CloseAllDoors"))
		auto d = Cast<ADoors>(door);
		d->Close();
	}
	for (AActor* dbtn : doorBtns)
	{
		auto db = Cast<ADoorButton>(dbtn);
		//문 닫히면 빨간불
		db->buttonMesh->SetVectorParameterValueOnMaterials(FName("doorStateColor"), FVector4(0.505f, 0.015f, 0.00974f, 1));
	}
	bOpened = false;
	UE_LOG(LogTemp, Warning, TEXT("DoorManager::CloseAllDoors"))
}

