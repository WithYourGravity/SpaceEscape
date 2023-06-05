// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreePathFinding.h"

#include "PuzzleRoomThreePathLocation.h"
#include "Kismet/GameplayStatics.h"

APuzzleRoomThreePathFinding::APuzzleRoomThreePathFinding()
{
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);
	sceneComp->SetRelativeScale3D(FVector(0.4f));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_PathFindingGround.SM_PathFindingGround'"));

	for(int i = 0; i < (width * length); i++)
	{
		FString compName = "groundBox" + FString::FromInt(i);
		FVector boxLoc = FVector((i / width) * 100, (i % width) * 100, 0);
		groundBoxArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*compName)));
		groundBoxArray[i]->SetupAttachment(sceneComp);
		groundBoxArray[i]->SetStaticMesh(tempMesh.Object);
		groundBoxArray[i]->SetRelativeLocation(boxLoc);
		groundBoxArray[i]->SetCollisionProfileName(FName("NoCollision"));
	}

	backPlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("backPlateMesh"));
	backPlateMesh->SetupAttachment(RootComponent);
	backPlateMesh->SetRelativeScale3D(FVector(18.f, 25.f, 1.f));
	backPlateMesh->SetRelativeLocation(FVector(650.f, 950.f, -200.f));
	backPlateMesh->SetCollisionProfileName(FName("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempBackPlate(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/Plane1.Plane1'"));
	if (tempBackPlate.Succeeded())
	{
		backPlateMesh->SetStaticMesh(tempBackPlate.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundBase>tempSuccessSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/pathfinding_success.pathfinding_success'"));
	if (tempSuccessSound.Succeeded())
	{
		successSound = tempSuccessSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase>tempFailSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/TouchPad_Failed.TouchPad_Failed'"));
	if (tempFailSound.Succeeded())
	{
		failSound = tempFailSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase>tempMoveSound(TEXT("/Script/Engine.SoundWave'/Game/LTG/Assets/Sound/pathFindBlock.pathFindBlock'"));
	if (tempMoveSound.Succeeded())
	{
		moveSound = tempMoveSound.Object;
	}
}

void APuzzleRoomThreePathFinding::BeginPlay()
{
	Super::BeginPlay();

	ResetThisPuzzle();

	pathLoc = Cast<APuzzleRoomThreePathLocation>(UGameplayStatics::GetActorOfClass(this, APuzzleRoomThreePathLocation::StaticClass()));

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

	if (bEndingMoveTrigger)
	{
		EndingMovingAtTick(DeltaSeconds);
	}

	if (bMakePathTrigger)
	{
		EndingMakePathAtTick(DeltaSeconds);
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
	lerpTime += deltaTime * blockMoveSpeed;
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
	// 전체 색 초기화
	for (UStaticMeshComponent* blocks : groundBoxArray)
	{
		blocks->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myGreen);
	}

	// 시작점 깜빡임 처리
	groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 0);

	// 시작행과 끝행에서 랜덤하게 픽
	beginPointIndex = FMath::RandRange(0, width - 1);
	endPointIndex = (width * length - 1) - FMath::RandRange(0, width - 1);

	// 픽된 블록 색상 변경
	groundBoxArray[beginPointIndex]->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myWhite);
	groundBoxArray[endPointIndex]->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myRed);

	// 조이스틱 연동 부분
	PlayedPathArray.Empty();
	playingNodeIndex = beginPointIndex;
	PlayedPathArray.Add(playingNodeIndex);
	groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 1);
}

// 퍼즐이 답이 있을 때까지 계속 리셋하는 함수
void APuzzleRoomThreePathFinding::ResetThisPuzzle()
{
	// 경로가 있을 때까지 리셋되는걸 보여주기
	/*
	GetWorldTimerManager().SetTimer(resetHandle, FTimerDelegate::CreateLambda([&]()
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
			GetWorldTimerManager().ClearTimer(resetHandle);
		}
	}), 1.f, true);
	*/

	// 경로가 생성되기 전까지 유저에게 보여주지 않음
	do
	{
		ResetBeginAndEndPoint();
		PickBoxRandomly(NumberOfPopUpBox);
	}
	while (!LetsFindPath());

	MovingTrigger();
}

void APuzzleRoomThreePathFinding::ResetRegularlyUpCountForResetPuzzle()
{
	regularlyUpCount = 0;
}

bool APuzzleRoomThreePathFinding::GetbisMoving()
{
	return bIsMoving;
}

// 박스를 이동시키는 함수를 발동시키는 함수
void APuzzleRoomThreePathFinding::MovingTrigger()
{
	// 이동방향 설정
	if (!bUpAndDown)
	{
		zPos = -400;
		bUpAndDown = true;
		bVisibility = true;
	}
	else
	{
		zPos = 400;
		bUpAndDown = false;
		bVisibility = false;
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
	// 초기화
	possibleNodeList.Empty();
	pickedNodeList.Empty();

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
	return true;
}

// 구한 최단경로를 배열에 저장하고 시각화해주는 함수
void APuzzleRoomThreePathFinding::PathLight()
{
	// 이전에 플레이한 정답루트 초기화
	AnswerPathArray.Empty();
	// 도착점 먼저 추가
	AnswerPathArray.Add(pickedNodeList[pickedNodeList.Num() - 1].nodeIndex);
	// 도착점의 부모노드 구함
	parentIndex = GetYourParentIndex(pickedNodeList[pickedNodeList.Num() - 1].nodeIndex);
	
	while (parentIndex != beginPointIndex)
	{
		// 배열에 최단거리루트 기록
		AnswerPathArray.Add(parentIndex);
		parentIndex = GetYourParentIndex(parentIndex);
	}

	// begin부터 정답불 들어오게
	AnswerPathArray.Add(beginPointIndex);

	Algo::Reverse(AnswerPathArray);

	//UE_LOG(LogTemp, Warning, TEXT("played array num : %d"), PlayedPathArray.Num());
	//UE_LOG(LogTemp, Warning, TEXT("answer array num : %d"), AnswerPathArray.Num());
	// 만약 알고리즘이 구한 최단거리와 플레이어가 플레이한 거리가 같다면(정답이라면)
	// 최단경로가 여러개일 수 있으므로 플레이어 경로를 보여주기로 한다
	if (PlayedPathArray.Num() == AnswerPathArray.Num())
	{
		AnswerPathArray = PlayedPathArray;
		ReportClear();
	}
	else
	{
		// 오답일 경우 플레이한 경로 초기화
		PlayedPathArray.Empty();
	}


	// 기록된 배열을 토대로 시각화
	GetWorldTimerManager().SetTimer(answerPathHandle, [&]()
		{
			groundBoxArray[AnswerPathArray[answerPathIndex]]->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myRed);

			// 이동 사운드 재생
			UGameplayStatics::PlaySound2D(this, moveSound, 0.6f);

			// 도착점에 도착했다면 타이머 클리어하고 다음을 위해 인덱스 초기화
			if (answerPathIndex == AnswerPathArray.Num() - 1)
			{
				GetWorldTimerManager().ClearTimer(answerPathHandle);
				answerPathIndex = 0;

				// 도착점까지 다 보여줬고 정답이었다면
				if(GetbWasReport())
				{
					// 퍼즐완료 효과
					FTimerHandle endingEffectHandle;
					GetWorldTimerManager().SetTimer(endingEffectHandle, this, &APuzzleRoomThreePathFinding::EndingEffect, 1.f, false);
				}
				else
				{
					// 도착점까지 다 보여줬고 오답이었다면
					// 전체 색 초기화
					for (UStaticMeshComponent* blocks : groundBoxArray)
					{
						blocks->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myRed);
					}
					MovingTrigger();

					// 실패 사운드 재생
					UGameplayStatics::PlaySound2D(this, failSound);
					FTimerHandle resetHandle;
					GetWorldTimerManager().SetTimer(resetHandle, this, &APuzzleRoomThreePathFinding::ResetThisPuzzle, 2, false);
				}
			}
			else
			{
				answerPathIndex++;
			}

		}, 0.2f, true);
}

// 플레이 가능한 블록인지 확인하고 playing노드를 해당 노드로 변경
bool APuzzleRoomThreePathFinding::CheckPlayable(EMoveDir direction)
{
	int orderedNodeIndex;

	switch (direction) {
		case EMoveDir::Forward:
			orderedNodeIndex = playingNodeIndex + width;
			if (!selectedBoxIndexArray.Contains(orderedNodeIndex) && (orderedNodeIndex) < width * length && !PlayedPathArray.Contains(orderedNodeIndex))
			{
				// 플레잉 노드 깜빡이 꺼주기
				groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 0);
				// 플레잉 노드 인덱스 변경
				playingNodeIndex = orderedNodeIndex;
				return true;
			}
			break;
		case EMoveDir::Back: 
			orderedNodeIndex = playingNodeIndex - width;
			if (!selectedBoxIndexArray.Contains(orderedNodeIndex) && orderedNodeIndex >= 0 && !PlayedPathArray.Contains(orderedNodeIndex))
			{
				groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 0);
				playingNodeIndex = orderedNodeIndex;
				return true;
			}
			break;
		case EMoveDir::Left:
			orderedNodeIndex = playingNodeIndex - 1;
			if (!selectedBoxIndexArray.Contains(orderedNodeIndex) && playingNodeIndex % width != 0 && !PlayedPathArray.Contains(orderedNodeIndex))
			{
				groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 0);
				playingNodeIndex = orderedNodeIndex;
				return true;
			}
			break;
		case EMoveDir::Right:
			orderedNodeIndex = playingNodeIndex + 1;
			if (!selectedBoxIndexArray.Contains(orderedNodeIndex) && (orderedNodeIndex) % width != 0 && !PlayedPathArray.Contains(orderedNodeIndex))
			{

				groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 0);
				playingNodeIndex = orderedNodeIndex;
				return true;
			}
			break;
	}
	return false;
}

void APuzzleRoomThreePathFinding::MovePlayingNode(EMoveDir direction)
{
	if (!CheckPlayable(direction))
	{
		return;
	}
	// playing 노드를 play array에 추가
	PlayedPathArray.Add(playingNodeIndex);
	// playing 노드 색 바꿔주기
	groundBoxArray[playingNodeIndex]->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myWhite);
	// 이동 사운드 재생
	UGameplayStatics::PlaySound2D(this, moveSound, 0.6f);

	if (playingNodeIndex == endPointIndex)
	{
		// 엔딩 처리
		PathLight();
	}
	else
	{
		// playing 노드 깜빡이 켜주기
		groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 1);
	}
}

void APuzzleRoomThreePathFinding::EndingEffect()
{
	//MovingTrigger();
	backPlateMesh->SetVisibility(false);

	// 전체 색 초기화
	for (UStaticMeshComponent* blocks : groundBoxArray)
	{
		blocks->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myWhite);
	}

	// 성공사운드 재생
	UGameplayStatics::PlaySound2D(this, successSound);

	lerpTime = 0;
	bEndingMoveTrigger = true;
}

void APuzzleRoomThreePathFinding::EndingMovingAtTick(float deltatime)
{
	lerpTime += deltatime * 0.6f;

	if (forEndingLerp > 0.98f)
	{
		// 랜덤위치 이동완료시 관련변수 초기화 하고 길만들기 트리거 실행
		forEndingLerp = 0;
		lerpTime = 0;
		countForRecordRandLoc = 0;
		newRandLoc.Empty();
		endingOriginLoc.Empty();
		bEndingMoveTrigger = false;
		bMakePathTrigger = true;
		//UE_LOG(LogTemp, Warning, TEXT("EndingMovingAtTick Complete!"));
		return;
	}
	else
	{
		forEndingLerp = 1 - FMath::Pow(1 - lerpTime, 5);
	}

	// 필요 없는 블럭 삭제하기
	if (!bDeleteOnce)
	{
		for (int i = (pathLoc->length * pathLoc->width) + 1; i < groundBoxArray.Num(); i++)
		{
			groundBoxArray[i]->SetScalarParameterValueOnMaterials(FName("Visibility"), 0);
			groundBoxArray.RemoveAt(i);
		}
		bDeleteOnce = true;
	}

	// 박스 움직이는 부분
	for (int i = 0; i < groundBoxArray.Num(); i++)
	{
		// 랜덤이동값 한번만 저장하게
		if (countForRecordRandLoc < groundBoxArray.Num())
		{
			newRandLoc.Add(groundBoxArray[i]->GetRelativeLocation() + (FMath::VRand() * 400));
			endingOriginLoc.Add(groundBoxArray[i]->GetRelativeLocation());
			countForRecordRandLoc++;
		}

		// 틱마다 이동부분
		groundBoxArray[i]->SetRelativeLocation(FMath::Lerp(endingOriginLoc[i], newRandLoc[i], forEndingLerp));
	}
}

void APuzzleRoomThreePathFinding::EndingMakePathAtTick(float deltatime)
{
	lerpTime += deltatime * 0.3f;

	if (forEndingLerp > 0.98f)
	{
		// 길 만들기 이동완료시
		forEndingLerp = 1;
		bMakePathTrigger = false;
		//UE_LOG(LogTemp, Warning, TEXT("MakePath Complete!"));

		// 길 완성시 설정해주기
		for (UStaticMeshComponent* blocks : groundBoxArray)
		{
			blocks->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)FLinearColor::Black);
			blocks->SetScalarParameterValueOnMaterials(FName("Visibility"), 1);
			blocks->SetScalarParameterValueOnMaterials(FName("WallOpacity"), 1);
			blocks->SetCollisionProfileName(FName("BlockAllDynamic"));
		}
	}
	else
	{
		//forEndingLerp = FMath::Sin(lerpTime);
		forEndingLerp = FMath::Pow(lerpTime, 4);
	}

	//UE_LOG(LogTemp, Warning, TEXT("lerptime : %f"), lerpTime);
	//UE_LOG(LogTemp, Warning, TEXT("forMakingPathLerp : %f"), forEndingLerp);

	for (int i = 0; i < groundBoxArray.Num(); i++)
	{
		// 비워진 랜덤이동값 배열에 길 위치 넣어주기
		if (countForRecordRandLoc < groundBoxArray.Num())
		{
			FVector pathBlockLoc = pathLoc->GetActorLocation();
			pathBlockLoc.X += 100 * GetActorRelativeScale3D().X * (i % pathLoc->width);
			pathBlockLoc.Y += 100 * GetActorRelativeScale3D().X * (i / pathLoc->width);
			
			newRandLoc.Add(pathBlockLoc);
			endingOriginLoc.Add(groundBoxArray[i]->GetComponentLocation());
			countForRecordRandLoc++;
		}

		// 틱마다 이동부분
		groundBoxArray[i]->SetWorldLocation(FMath::Lerp(endingOriginLoc[i], newRandLoc[i], forEndingLerp));
	}
}

