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

// 장애물이 될 박스를 랜덤하게 고르는 함수
void APuzzleRoomThreePathFinding::PickBoxRandomly(int number)
{
	// 초기화
	selectedBoxIndexArray.Empty();

	for(int i = 0; i < number; i++)
	{
		// 랜덤하게 골라서
		int idx = FMath::RandRange(0, groundBoxArray.Num() - 1);

		// 중복된거, 시작 & 끝점 선택되면 다시
		if (selectedBoxIndexArray.Contains(idx) || idx == beginPointIndex || idx == endPointIndex)
		{
			i--;
			continue;
		}
		selectedBoxIndexArray.Add(idx);
	}
}

// 선택된 박스를 이동시키게끔 틱마다 실행되는 함수
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

// 시작점과 끝점을 다시 고르는 함수
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

// 퍼즐이 답이 있을 때까지 계속 리셋하는 함수
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

// 박스를 이동시키는 함수를 발동시키는 함수
void APuzzleRoomThreePathFinding::MovingTrigger()
{
	// 이동방향 설정
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

// 현재 노드를 넣으면 이동가능한지 판별하고 possible리스트에 추가하는 함수
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

// 해당 노드가 이동가능한지 판별해 bool값을 넘겨주는 함수
bool APuzzleRoomThreePathFinding::CheckMovable(int nextNodeIndex)
{
	// 장애물로 선택된 노드가 아닌지
	if (selectedBoxIndexArray.Contains(nextNodeIndex))
	{
		return false;
	}

	// 이미 픽된 노드가 아닌지
	// 추후 갱신조건 추가해야 함
	for (int i = 0; i < pickedNodeList.Num(); i++)
	{
		if (pickedNodeList[i].nodeIndex == nextNodeIndex)
		{
			return false;
		}
	}

	return true;
}

// 구조체를 하나 만들어 index를 넣고 possible리스트에 추가하는 함수
void APuzzleRoomThreePathFinding::AddIndexPossibleList(int nodeIndex)
{
	// 임시 구조체 만들고 정보 넣어주기
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

	// 이미 해당 노드의 구조체가 존재하는 경우
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

	// 리스트에 추가
	possibleNodeList.Add(tempStruct);
}

// possible리스트에 있는 노드 중 가장 F값이 낮은 노드를 Picked리스트로 옮기는 함수
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

// 노드인덱스를 전달하면 부모인덱스를 반환하는 함수 (배열에 존재하지 않는 노드라면 -1반환)
int APuzzleRoomThreePathFinding::GetYourParentIndex(int childNodeIndex)
{
	// possible 리스트와 picked 리스트에서 해당 인덱스의 구조체를 찾는다
	for (int i = 0; i < possibleNodeList.Num(); i++)
	{
		if (possibleNodeList[i].nodeIndex == childNodeIndex)
		{
			// 그 구조체의 parent 인덱스 값을 반환한다
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

// 길찾기 알고리즘 실행함수
bool APuzzleRoomThreePathFinding::LetsFindPath()
{
	// 시작점을 현재노드로 기록하고 리스트에 추가
	currentNodeIndex = beginPointIndex;
	AddIndexPossibleList(beginPointIndex);

	AddBestIndexPickedList();

	// 도착점에 이를 때까지 계속 반복
	while (pickedNodeList[pickedNodeList.Num() - 1].nodeIndex != endPointIndex)
	{
		// 접근 가능한 노드 탐색
		FindPossibleNode(currentNodeIndex);

		// 이동 불가시 
		if (possibleNodeList.IsEmpty())
		{
			return false;
		}

		// 가장 F값 낮은 노드 선택
		AddBestIndexPickedList();
	}

	PathLight();
	return true;
}

// 구한 최단경로를 배열에 저장하고 시각화해주는 함수
void APuzzleRoomThreePathFinding::PathLight()
{
	parentIndex = GetYourParentIndex(pickedNodeList[pickedNodeList.Num() - 1].nodeIndex);
	
	while (parentIndex != beginPointIndex)
	{
		// 배열에 최단거리루트 기록
		AnswerPathArray.Add(parentIndex);
		parentIndex = GetYourParentIndex(parentIndex);
	}

	Algo::Reverse(AnswerPathArray);

	// 기록된 배열을 토대로 시각화
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

