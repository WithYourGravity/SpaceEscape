// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreePathFinding.h"

APuzzleRoomThreePathFinding::APuzzleRoomThreePathFinding()
{
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);
	sceneComp->SetRelativeScale3D(FVector(0.2f));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_PathFindingGround.SM_PathFindingGround'"));
	
	for(int i = 0; i < width * length; i++)
	{
		FString compName = "groundBox" + FString::FromInt(i);
		FVector boxLoc = FVector((i / width) * 100, (i % width) * 100, 0);
		groundBoxArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*compName)));
		groundBoxArray[i]->SetupAttachment(sceneComp);
		groundBoxArray[i]->SetStaticMesh(tempMesh.Object);
		groundBoxArray[i]->SetRelativeLocation(boxLoc);
	}
}

void APuzzleRoomThreePathFinding::BeginPlay()
{
	Super::BeginPlay();

	ResetBeginAndEndPoint();
	PickBoxRandomly(NumberOfPopUpBox);
	MovingTrigger();

	/*
	GetWorldTimerManager().SetTimer(hd, FTimerDelegate::CreateLambda([&]()
	{
		if (regularlyUpCount % 2 == 0)
		{
			ResetBeginAndEndPoint();
			PickBoxRandomly(NumberOfPopUpBox);
		}
		MovingTrigger();
		regularlyUpCount++;
	}), 4, true);
	*/
}

void APuzzleRoomThreePathFinding::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsMoving)
	{
		MovingFunctionAtTick(DeltaSeconds);
	}
}

// ��ֹ��� �� �ڽ��� �����ϰ� ���� �Լ�
void APuzzleRoomThreePathFinding::PickBoxRandomly(int number)
{
	// �ʱ�ȭ
	selectedBoxIndexArray.Empty();

	for(int i = 0; i < number; i++)
	{
		// �����ϰ� ���
		int idx = FMath::RandRange(0, groundBoxArray.Num() - 1);

		// �ߺ��Ȱ�, ���� & ���� ���õǸ� �ٽ�
		if (selectedBoxIndexArray.Contains(idx) || idx == beginPointIndex || idx == endPointIndex)
		{
			i--;
			continue;
		}
		selectedBoxIndexArray.Add(idx);
	}
}

// ���õ� �ڽ��� �̵���Ű�Բ� ƽ���� ����Ǵ� �Լ�
void APuzzleRoomThreePathFinding::MovingFunctionAtTick(float deltaTime)
{
	lerpTime += deltaTime;
	if (lerpTime > 0.95)
	{
		lerpTime = 1;
		bIsMoving = false;
	}

	for(int i = 0; i < selectedBoxIndexArray.Num(); i++)
	{
		UStaticMeshComponent* selectedMeshComp = groundBoxArray[selectedBoxIndexArray[i]];
		if (countForRecordStartLoc <= selectedBoxIndexArray.Num())
		{
			startLocArray.Add(selectedMeshComp->GetRelativeLocation());
			countForRecordStartLoc++;
		}
		selectedMeshComp->SetRelativeLocation(FMath::Lerp(startLocArray[i], startLocArray[i] + FVector(0, 0, zPos), lerpTime));
	}
}

// �������� ������ �ٽ� ���� �Լ�
void APuzzleRoomThreePathFinding::ResetBeginAndEndPoint()
{
	if (beginPointIndex != 0)
	{
		groundBoxArray[beginPointIndex]->SetVectorParameterValueOnMaterials(FName("BoxParam"), (FVector)FColor::Yellow);
		groundBoxArray[endPointIndex]->SetVectorParameterValueOnMaterials(FName("BoxParam"), (FVector)FColor::Yellow);
	}
	beginPointIndex = FMath::RandRange(0, width - 1);
	endPointIndex = (width * length - 1) - FMath::RandRange(0, width - 1);
	groundBoxArray[beginPointIndex]->SetVectorParameterValueOnMaterials(FName("BoxParam"), (FVector)FColor::Red);
	groundBoxArray[endPointIndex]->SetVectorParameterValueOnMaterials(FName("BoxParam"), (FVector)FColor::Blue);
}

// �ڽ��� �̵���Ű�� �Լ��� �ߵ���Ű�� �Լ�
void APuzzleRoomThreePathFinding::MovingTrigger()
{
	// �̵����� ����
	if (groundBoxArray[selectedBoxIndexArray[0]]->GetRelativeLocation().Z < 10)
	{
		zPos = 100;
	}
	else
	{
		zPos = -100;
	}

	countForRecordStartLoc = 0;
	startLocArray.Empty();
	lerpTime = 0;
	bIsMoving = true;
}
