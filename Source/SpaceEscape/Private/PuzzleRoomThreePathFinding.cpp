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

// �������� ������ �ٽ� ���� �Լ�
void APuzzleRoomThreePathFinding::ResetBeginAndEndPoint()
{
	// ��ü �� �ʱ�ȭ
	for (UStaticMeshComponent* blocks : groundBoxArray)
	{
		blocks->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myGreen);
	}

	// ������ ������ ó��
	groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 0);

	// ������� ���࿡�� �����ϰ� ��
	beginPointIndex = FMath::RandRange(0, width - 1);
	endPointIndex = (width * length - 1) - FMath::RandRange(0, width - 1);

	// �ȵ� ��� ���� ����
	groundBoxArray[beginPointIndex]->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myWhite);
	groundBoxArray[endPointIndex]->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myRed);

	// ���̽�ƽ ���� �κ�
	PlayedPathArray.Empty();
	playingNodeIndex = beginPointIndex;
	PlayedPathArray.Add(playingNodeIndex);
	groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 1);
}

// ������ ���� ���� ������ ��� �����ϴ� �Լ�
void APuzzleRoomThreePathFinding::ResetThisPuzzle()
{
	// ��ΰ� ���� ������ ���µǴ°� �����ֱ�
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

	// ��ΰ� �����Ǳ� ������ �������� �������� ����
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

// �ڽ��� �̵���Ű�� �Լ��� �ߵ���Ű�� �Լ�
void APuzzleRoomThreePathFinding::MovingTrigger()
{
	// �̵����� ����
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
	// �ʱ�ȭ
	possibleNodeList.Empty();
	pickedNodeList.Empty();

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
	return true;
}

// ���� �ִܰ�θ� �迭�� �����ϰ� �ð�ȭ���ִ� �Լ�
void APuzzleRoomThreePathFinding::PathLight()
{
	// ������ �÷����� �����Ʈ �ʱ�ȭ
	AnswerPathArray.Empty();
	// ������ ���� �߰�
	AnswerPathArray.Add(pickedNodeList[pickedNodeList.Num() - 1].nodeIndex);
	// �������� �θ��� ����
	parentIndex = GetYourParentIndex(pickedNodeList[pickedNodeList.Num() - 1].nodeIndex);
	
	while (parentIndex != beginPointIndex)
	{
		// �迭�� �ִܰŸ���Ʈ ���
		AnswerPathArray.Add(parentIndex);
		parentIndex = GetYourParentIndex(parentIndex);
	}

	// begin���� ����� ������
	AnswerPathArray.Add(beginPointIndex);

	Algo::Reverse(AnswerPathArray);

	//UE_LOG(LogTemp, Warning, TEXT("played array num : %d"), PlayedPathArray.Num());
	//UE_LOG(LogTemp, Warning, TEXT("answer array num : %d"), AnswerPathArray.Num());
	// ���� �˰����� ���� �ִܰŸ��� �÷��̾ �÷����� �Ÿ��� ���ٸ�(�����̶��)
	// �ִܰ�ΰ� �������� �� �����Ƿ� �÷��̾� ��θ� �����ֱ�� �Ѵ�
	if (PlayedPathArray.Num() == AnswerPathArray.Num())
	{
		AnswerPathArray = PlayedPathArray;
		ReportClear();
	}
	else
	{
		// ������ ��� �÷����� ��� �ʱ�ȭ
		PlayedPathArray.Empty();
	}


	// ��ϵ� �迭�� ���� �ð�ȭ
	GetWorldTimerManager().SetTimer(answerPathHandle, [&]()
		{
			groundBoxArray[AnswerPathArray[answerPathIndex]]->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myRed);

			// �̵� ���� ���
			UGameplayStatics::PlaySound2D(this, moveSound, 0.6f);

			// �������� �����ߴٸ� Ÿ�̸� Ŭ�����ϰ� ������ ���� �ε��� �ʱ�ȭ
			if (answerPathIndex == AnswerPathArray.Num() - 1)
			{
				GetWorldTimerManager().ClearTimer(answerPathHandle);
				answerPathIndex = 0;

				// ���������� �� ������� �����̾��ٸ�
				if(GetbWasReport())
				{
					// ����Ϸ� ȿ��
					FTimerHandle endingEffectHandle;
					GetWorldTimerManager().SetTimer(endingEffectHandle, this, &APuzzleRoomThreePathFinding::EndingEffect, 1.f, false);
				}
				else
				{
					// ���������� �� ������� �����̾��ٸ�
					// ��ü �� �ʱ�ȭ
					for (UStaticMeshComponent* blocks : groundBoxArray)
					{
						blocks->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myRed);
					}
					MovingTrigger();

					// ���� ���� ���
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

// �÷��� ������ ������� Ȯ���ϰ� playing��带 �ش� ���� ����
bool APuzzleRoomThreePathFinding::CheckPlayable(EMoveDir direction)
{
	int orderedNodeIndex;

	switch (direction) {
		case EMoveDir::Forward:
			orderedNodeIndex = playingNodeIndex + width;
			if (!selectedBoxIndexArray.Contains(orderedNodeIndex) && (orderedNodeIndex) < width * length && !PlayedPathArray.Contains(orderedNodeIndex))
			{
				// �÷��� ��� ������ ���ֱ�
				groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 0);
				// �÷��� ��� �ε��� ����
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
	// playing ��带 play array�� �߰�
	PlayedPathArray.Add(playingNodeIndex);
	// playing ��� �� �ٲ��ֱ�
	groundBoxArray[playingNodeIndex]->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myWhite);
	// �̵� ���� ���
	UGameplayStatics::PlaySound2D(this, moveSound, 0.6f);

	if (playingNodeIndex == endPointIndex)
	{
		// ���� ó��
		PathLight();
	}
	else
	{
		// playing ��� ������ ���ֱ�
		groundBoxArray[playingNodeIndex]->SetScalarParameterValueOnMaterials(FName("BlinkOn"), 1);
	}
}

void APuzzleRoomThreePathFinding::EndingEffect()
{
	//MovingTrigger();
	backPlateMesh->SetVisibility(false);

	// ��ü �� �ʱ�ȭ
	for (UStaticMeshComponent* blocks : groundBoxArray)
	{
		blocks->SetVectorParameterValueOnMaterials(FName("BaseColor"), (FVector)myWhite);
	}

	// �������� ���
	UGameplayStatics::PlaySound2D(this, successSound);

	lerpTime = 0;
	bEndingMoveTrigger = true;
}

void APuzzleRoomThreePathFinding::EndingMovingAtTick(float deltatime)
{
	lerpTime += deltatime * 0.6f;

	if (forEndingLerp > 0.98f)
	{
		// ������ġ �̵��Ϸ�� ���ú��� �ʱ�ȭ �ϰ� �游��� Ʈ���� ����
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

	// �ʿ� ���� �� �����ϱ�
	if (!bDeleteOnce)
	{
		for (int i = (pathLoc->length * pathLoc->width) + 1; i < groundBoxArray.Num(); i++)
		{
			groundBoxArray[i]->SetScalarParameterValueOnMaterials(FName("Visibility"), 0);
			groundBoxArray.RemoveAt(i);
		}
		bDeleteOnce = true;
	}

	// �ڽ� �����̴� �κ�
	for (int i = 0; i < groundBoxArray.Num(); i++)
	{
		// �����̵��� �ѹ��� �����ϰ�
		if (countForRecordRandLoc < groundBoxArray.Num())
		{
			newRandLoc.Add(groundBoxArray[i]->GetRelativeLocation() + (FMath::VRand() * 400));
			endingOriginLoc.Add(groundBoxArray[i]->GetRelativeLocation());
			countForRecordRandLoc++;
		}

		// ƽ���� �̵��κ�
		groundBoxArray[i]->SetRelativeLocation(FMath::Lerp(endingOriginLoc[i], newRandLoc[i], forEndingLerp));
	}
}

void APuzzleRoomThreePathFinding::EndingMakePathAtTick(float deltatime)
{
	lerpTime += deltatime * 0.3f;

	if (forEndingLerp > 0.98f)
	{
		// �� ����� �̵��Ϸ��
		forEndingLerp = 1;
		bMakePathTrigger = false;
		//UE_LOG(LogTemp, Warning, TEXT("MakePath Complete!"));

		// �� �ϼ��� �������ֱ�
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
		// ����� �����̵��� �迭�� �� ��ġ �־��ֱ�
		if (countForRecordRandLoc < groundBoxArray.Num())
		{
			FVector pathBlockLoc = pathLoc->GetActorLocation();
			pathBlockLoc.X += 100 * GetActorRelativeScale3D().X * (i % pathLoc->width);
			pathBlockLoc.Y += 100 * GetActorRelativeScale3D().X * (i / pathLoc->width);
			
			newRandLoc.Add(pathBlockLoc);
			endingOriginLoc.Add(groundBoxArray[i]->GetComponentLocation());
			countForRecordRandLoc++;
		}

		// ƽ���� �̵��κ�
		groundBoxArray[i]->SetWorldLocation(FMath::Lerp(endingOriginLoc[i], newRandLoc[i], forEndingLerp));
	}
}

