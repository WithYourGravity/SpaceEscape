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

	// ������ Ŭ���� �Ǹ� ��������Ʈ ���ε��� �Լ� ����
	for (TActorIterator<APuzzleBase> it(GetWorld()); it; ++it)
	{
		APuzzleBase* pz = *it;
		pz->puzzleClearDele.BindUFunction(this, FName("AddSolvedPuzzleCountForDoorLight"));
	}

	// �� �ʱ�ȭ
	ChangeColorByCount();
}

// Called every frame
void ADoorLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ������ Ǯ�� ������ ī��Ʈ�� �÷��ִ� �Լ�
void ADoorLight::AddSolvedPuzzleCountForDoorLight()
{
	if (solvedPuzzleCountForDoorLight > 2)
	{
		return;
	}
	solvedPuzzleCountForDoorLight++;
	ChangeColorByCount();
}

// Ǯ�� ������ ī��Ʈ�� ���� �Һ��� ������ �ٲٴ� �Լ�
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

