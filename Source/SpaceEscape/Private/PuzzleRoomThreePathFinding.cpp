// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreePathFinding.h"

APuzzleRoomThreePathFinding::APuzzleRoomThreePathFinding()
{
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);
	sceneComp->SetRelativeScale3D(FVector(0.2f));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_PathFindingGround.SM_PathFindingGround'"));

	for(int i = 0; i < (width * length); i++)
	{
		FString compName = "groundBox" + FString::FromInt(i);
		FVector boxLoc = FVector((i / width) * 100, (i % width) * 100, 0);
		groundBoxArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*compName)));
		groundBoxArray[i]->SetupAttachment(sceneComp);
		groundBoxArray[i]->SetStaticMesh(tempMesh.Object);
		groundBoxArray[i]->SetRelativeLocation(boxLoc);
	}

	UE_LOG(LogTemp, Warning, TEXT("groundBoxArray Num : %d"), groundBoxArray.Num());
}

void APuzzleRoomThreePathFinding::BeginPlay()
{
	Super::BeginPlay();

	ResetThisPuzzle();

	// find node test
	//UE_LOG(LogTemp, Warning, TEXT("is reached : %s"), LetsFindPath() ? TEXT("Yes") : TEXT("No"));
	/*
	UE_LOG(LogTemp, Warning, TEXT("possible node list num : %d"), possibleNodeList.Num());
	for (int i = 0; i < possibleNodeList.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("node index : %d"), possibleNodeList[i].nodeIndex);
		UE_LOG(LogTemp, Warning, TEXT("parentNodeIndex : %d"), possibleNodeList[i].parentNodeIndex);
		UE_LOG(LogTemp, Warning, TEXT("gValue : %f"), possibleNodeList[i].gValue);
		UE_LOG(LogTemp, Warning, TEXT("hValue : %f"), possibleNodeList[i].hValue);
		UE_LOG(LogTemp, Warning, TEXT("fValue : %f"), possibleNodeList[i].fValue);
	}
	*/
	/*
	UE_LOG(LogTemp, Warning, TEXT("picked node list num : %d"), pickedNodeList.Num());
	for (int i = 0; i < pickedNodeList.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("node index : %d\nparentNode : %d\ngVal : %f\nhVal : %f\nfVal : %f"), pickedNodeList[i].nodeIndex, pickedNodeList[i].parentNodeIndex, pickedNodeList[i].gValue, pickedNodeList[i].hValue, pickedNodeList[i].fValue);
	}
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
	if (beginPointIndex != -1)
	{
		groundBoxArray[beginPointIndex]->SetVectorParameterValueOnMaterials(FName("BoxParam"), (FVector)FColor::Yellow);
		groundBoxArray[endPointIndex]->SetVectorParameterValueOnMaterials(FName("BoxParam"), (FVector)FColor::Yellow);
	}
	beginPointIndex = FMath::RandRange(0, width - 1);
	endPointIndex = (width * length - 1) - FMath::RandRange(0, width - 1);
	groundBoxArray[beginPointIndex]->SetVectorParameterValueOnMaterials(FName("BoxParam"), (FVector)FColor::Red);
	groundBoxArray[endPointIndex]->SetVectorParameterValueOnMaterials(FName("BoxParam"), (FVector)FColor::Blue);
}

// ������ ���� ���� ������ ��� �����ϴ� �Լ�
void APuzzleRoomThreePathFinding::ResetThisPuzzle()
{
	GetWorldTimerManager().SetTimer(hd, FTimerDelegate::CreateLambda([&]()
	{
		if (regularlyUpCount % 2 == 0)
		{
			ResetBeginAndEndPoint();
			PickBoxRandomly(NumberOfPopUpBox);
		}
		MovingTrigger();
		regularlyUpCount++;

		if (LetsFindPath() == true)
		{
			GetWorldTimerManager().ClearTimer(hd);
		}
	}), 2, true);
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

// ���� ��带 ������ �̵��������� �Ǻ��ϰ� possible����Ʈ�� �߰��ϴ� �Լ�
bool APuzzleRoomThreePathFinding::FindPossibleNode(int nodeIndex)
{
	int count = 0;

	if (CheckMovable(nodeIndex + 1) && (nodeIndex + 1) % width != 0)
	{
		AddIndexPossibleList(nodeIndex + 1);
		count++;
	}

	if (CheckMovable(nodeIndex - 1) && nodeIndex % width != 0)
	{
		AddIndexPossibleList(nodeIndex - 1);
		count++;
	}

	if (CheckMovable(nodeIndex + width) && (nodeIndex + width) < width * length)
	{
		AddIndexPossibleList(nodeIndex + width);
		count++;
	}

	if (CheckMovable(nodeIndex - width) && nodeIndex - width >= 0)
	{
		AddIndexPossibleList(nodeIndex - width);
		count++;
	}

	if (count == 0)
	{
		return false;
	}

	return true;

}

// �ش� ��尡 �̵��������� �Ǻ��� bool���� �Ѱ��ִ� �Լ�
bool APuzzleRoomThreePathFinding::CheckMovable(int nextNodeIndex)
{
	// ��ֹ��� ���õ� ��尡 �ƴ���
	if (selectedBoxIndexArray.Contains(nextNodeIndex))
	{
		return false;
	}

	// �̹� �ȵ� ��尡 �ƴ���
	// ���� �������� �߰��ؾ� ��
	for (int i = 0; i < pickedNodeList.Num(); i++)
	{
		if (pickedNodeList[i].nodeIndex == nextNodeIndex)
		{
			return false;
		}
	}

	return true;
}

// ����ü�� �ϳ� ����� index�� �ְ� possible����Ʈ�� �߰��ϴ� �Լ�
void APuzzleRoomThreePathFinding::AddIndexPossibleList(int nodeIndex)
{
	// �ӽ� ����ü ����� ���� �־��ֱ�
	FNodeInfo tempStruct;
	tempStruct.nodeIndex = nodeIndex;
	tempStruct.parentNodeIndex = currentNodeIndex;

	tempStruct.gValue = 100;
	int findingNodeIndex = currentNodeIndex;
	while (findingNodeIndex != beginPointIndex)
	{
		findingNodeIndex = GetYourParentIndex(findingNodeIndex);
		tempStruct.gValue += 100;
	}
	if (nodeIndex == beginPointIndex)
	{
		tempStruct.gValue = 0;
	}
	tempStruct.hValue = FVector::Dist(groundBoxArray[nodeIndex]->GetRelativeLocation(), groundBoxArray[endPointIndex]->GetRelativeLocation());
	tempStruct.fValue = tempStruct.gValue + tempStruct.hValue;

	// �̹� �ش� ����� ����ü�� �����ϴ� ���
	for (int i = 0; i < possibleNodeList.Num(); i++)
	{
		if (possibleNodeList[i].nodeIndex == nodeIndex)
		{
			if (possibleNodeList[i].gValue <= tempStruct.gValue)
			{
				return;
			}
			else
			{
				possibleNodeList[i].gValue = tempStruct.gValue;
			}
		}
	}

	// ����Ʈ�� �߰�
	possibleNodeList.Add(tempStruct);
}

// possible����Ʈ�� �ִ� ��� �� ���� F���� ���� ��带 Picked����Ʈ�� �ű�� �Լ�
void APuzzleRoomThreePathFinding::AddBestIndexPickedList()
{
	possibleNodeList.Sort([](const FNodeInfo& A, const FNodeInfo& B)
		{
			return A.fValue > B.fValue;
		});

	FNodeInfo tempInfo = possibleNodeList.Pop(false);
	currentNodeIndex = tempInfo.nodeIndex;

	pickedNodeList.Add(tempInfo);
}

// ����ε����� �����ϸ� �θ��ε����� ��ȯ�ϴ� �Լ� (�迭�� �������� �ʴ� ����� -1��ȯ)
int APuzzleRoomThreePathFinding::GetYourParentIndex(int childNodeIndex)
{
	// possible ����Ʈ�� picked ����Ʈ���� �ش� �ε����� ����ü�� ã�´�
	for (int i = 0; i < possibleNodeList.Num(); i++)
	{
		if (possibleNodeList[i].nodeIndex == childNodeIndex)
		{
			// �� ����ü�� parent �ε��� ���� ��ȯ�Ѵ�
			return possibleNodeList[i].parentNodeIndex;
		}
	}
	for (int i = 0; i < pickedNodeList.Num(); i++)
	{
		if (pickedNodeList[i].nodeIndex == childNodeIndex)
		{
			return pickedNodeList[i].parentNodeIndex;
		}
	}
	return -1;
}

// ��ã�� �˰��� �����Լ�
bool APuzzleRoomThreePathFinding::LetsFindPath()
{
	// �������� ������� ����ϰ� ����Ʈ�� �߰�
	currentNodeIndex = beginPointIndex;
	AddIndexPossibleList(beginPointIndex);

	AddBestIndexPickedList();

	// �������� �̸� ������ ��� �ݺ�
	while (pickedNodeList[pickedNodeList.Num() - 1].nodeIndex != endPointIndex)
	{
		// ���� ������ ��� Ž��
		FindPossibleNode(currentNodeIndex);

		// �̵� �Ұ��� 
		if (possibleNodeList.IsEmpty())
		{
			return false;
		}

		// ���� F�� ���� ��� ����
		AddBestIndexPickedList();
	}

	PathLight();
	return true;
}

// ���� �ִܰ�θ� �迭�� �����ϰ� �ð�ȭ���ִ� �Լ�
void APuzzleRoomThreePathFinding::PathLight()
{
	parentIndex = GetYourParentIndex(pickedNodeList[pickedNodeList.Num() - 1].nodeIndex);
	
	while (parentIndex != beginPointIndex)
	{
		// �迭�� �ִܰŸ���Ʈ ���
		AnswerPathArray.Add(parentIndex);
		parentIndex = GetYourParentIndex(parentIndex);
	}

	Algo::Reverse(AnswerPathArray);

	// ��ϵ� �迭�� ���� �ð�ȭ
	GetWorldTimerManager().SetTimer(answerPathHandle, [&]()
		{
			groundBoxArray[AnswerPathArray[answerPathIndex]]->SetVectorParameterValueOnMaterials(FName("BoxParam"), (FVector)FColor::Green);

			if (answerPathIndex == AnswerPathArray.Num() - 1)
			{
				GetWorldTimerManager().ClearTimer(answerPathHandle);
			}

			answerPathIndex++;
		}, 0.4f, true);
}

