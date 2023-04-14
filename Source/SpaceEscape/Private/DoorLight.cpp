// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorLight.h"
#include "EngineUtils.h"
#include "PuzzleBase.h"

// Sets default values
ADoorLight::ADoorLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(meshComp);
}

// Called when the game starts or when spawned
void ADoorLight::BeginPlay()
{
	Super::BeginPlay();

	// 퍼즐이 클리어 되면 델리게이트 바인딩된 함수 실행
	for (TActorIterator<APuzzleBase> it(GetWorld()); it; ++it)
	{
		APuzzleBase* pz = *it;
		pz->puzzleClearDele.BindUFunction(this, FName("AddSolvedPuzzleCountForDoorLight"));
	}

	// 색 초기화
	ChangeColorByCount();
}

// Called every frame
void ADoorLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 퍼즐이 풀릴 때마다 카운트를 올려주는 함수
void ADoorLight::AddSolvedPuzzleCountForDoorLight()
{
	if (solvedPuzzleCountForDoorLight > 2)
	{
		return;
	}
	solvedPuzzleCountForDoorLight++;
	ChangeColorByCount();
}

// 풀린 퍼즐의 카운트에 따라서 불빛의 색깔을 바꾸는 함수
void ADoorLight::ChangeColorByCount()
{
	switch ((ColorList)solvedPuzzleCountForDoorLight)
	{
	case ColorList::Red: 
		meshComp->SetVectorParameterValueOnMaterials(FName("ColorParam"), (FVector)FColor::Red);
		break;
	case ColorList::Yellow: 
		meshComp->SetVectorParameterValueOnMaterials(FName("ColorParam"), (FVector)FColor::Yellow);
		break;
	case ColorList::Green: 
		meshComp->SetVectorParameterValueOnMaterials(FName("ColorParam"), (FVector)FColor::Green);
		break;
	}
}

