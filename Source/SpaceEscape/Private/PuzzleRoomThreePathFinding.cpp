// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomThreePathFinding.h"

APuzzleRoomThreePathFinding::APuzzleRoomThreePathFinding()
{
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	SetRootComponent(sceneComp);
	sceneComp->SetRelativeScale3D(FVector(0.2f));

	groundBox1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox1"));
	groundBox2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox2"));
	groundBox3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox3"));
	groundBox4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox4"));
	groundBox5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox5"));
	groundBox6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox6"));
	groundBox7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox7"));
	groundBox8 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox8"));
	groundBox9 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox9"));
	groundBox10 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox10"));
	groundBox11 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox11"));
	groundBox12 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox12"));
	groundBox13 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox13"));
	groundBox14 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox14"));
	groundBox15 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox15"));
	groundBox16 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox16"));
	groundBox17 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox17"));
	groundBox18 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox18"));
	groundBox19 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox19"));
	groundBox20 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox20"));
	groundBox21 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox21"));
	groundBox22 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox22"));
	groundBox23 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox23"));
	groundBox24 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox24"));
	groundBox25 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox25"));
	groundBox26 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox26"));
	groundBox27 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox27"));
	groundBox28 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox28"));
	groundBox29 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox29"));
	groundBox30 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox30"));
	groundBox31 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox31"));
	groundBox32 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox32"));
	groundBox33 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox33"));
	groundBox34 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox34"));
	groundBox35 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox35"));
	groundBox36 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox36"));
	groundBox37 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox37"));
	groundBox38 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox38"));
	groundBox39 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox39"));
	groundBox40 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox40"));
	groundBox41 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox41"));
	groundBox42 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox42"));
	groundBox43 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox43"));
	groundBox44 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox44"));
	groundBox45 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox45"));
	groundBox46 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox46"));
	groundBox47 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox47"));
	groundBox48 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox48"));
	groundBox49 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox49"));
	groundBox50 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("groundBox50"));

	groundBox1->SetupAttachment(sceneComp);
	groundBox2->SetupAttachment(sceneComp);
	groundBox3->SetupAttachment(sceneComp);
	groundBox4->SetupAttachment(sceneComp);
	groundBox5->SetupAttachment(sceneComp);
	groundBox6->SetupAttachment(sceneComp);
	groundBox7->SetupAttachment(sceneComp);
	groundBox8->SetupAttachment(sceneComp);
	groundBox9->SetupAttachment(sceneComp);
	groundBox10->SetupAttachment(sceneComp);
	groundBox11->SetupAttachment(sceneComp);
	groundBox12->SetupAttachment(sceneComp);
	groundBox13->SetupAttachment(sceneComp);
	groundBox14->SetupAttachment(sceneComp);
	groundBox15->SetupAttachment(sceneComp);
	groundBox16->SetupAttachment(sceneComp);
	groundBox17->SetupAttachment(sceneComp);
	groundBox18->SetupAttachment(sceneComp);
	groundBox19->SetupAttachment(sceneComp);
	groundBox20->SetupAttachment(sceneComp);
	groundBox21->SetupAttachment(sceneComp);
	groundBox22->SetupAttachment(sceneComp);
	groundBox23->SetupAttachment(sceneComp);
	groundBox24->SetupAttachment(sceneComp);
	groundBox25->SetupAttachment(sceneComp);
	groundBox26->SetupAttachment(sceneComp);
	groundBox27->SetupAttachment(sceneComp);
	groundBox28->SetupAttachment(sceneComp);
	groundBox29->SetupAttachment(sceneComp);
	groundBox30->SetupAttachment(sceneComp);
	groundBox31->SetupAttachment(sceneComp);
	groundBox32->SetupAttachment(sceneComp);
	groundBox33->SetupAttachment(sceneComp);
	groundBox34->SetupAttachment(sceneComp);
	groundBox35->SetupAttachment(sceneComp);
	groundBox36->SetupAttachment(sceneComp);
	groundBox37->SetupAttachment(sceneComp);
	groundBox38->SetupAttachment(sceneComp);
	groundBox39->SetupAttachment(sceneComp);
	groundBox40->SetupAttachment(sceneComp);
	groundBox41->SetupAttachment(sceneComp);
	groundBox42->SetupAttachment(sceneComp);
	groundBox43->SetupAttachment(sceneComp);
	groundBox44->SetupAttachment(sceneComp);
	groundBox45->SetupAttachment(sceneComp);
	groundBox46->SetupAttachment(sceneComp);
	groundBox47->SetupAttachment(sceneComp);
	groundBox48->SetupAttachment(sceneComp);
	groundBox49->SetupAttachment(sceneComp);
	groundBox50->SetupAttachment(sceneComp);

	groundBox1->SetRelativeLocation(FVector(0, 0, 0));
	groundBox2->SetRelativeLocation(FVector(0, 100, 0));
	groundBox3->SetRelativeLocation(FVector(0, 200, 0));
	groundBox4->SetRelativeLocation(FVector(0, 300, 0));
	groundBox5->SetRelativeLocation(FVector(0, 400, 0));
	groundBox6->SetRelativeLocation(FVector(100, 0, 0));
	groundBox7->SetRelativeLocation(FVector(100, 100, 0));
	groundBox8->SetRelativeLocation(FVector(100, 200, 0));
	groundBox9->SetRelativeLocation(FVector(100, 300, 0));
	groundBox10->SetRelativeLocation(FVector(100, 400, 0));
	groundBox11->SetRelativeLocation(FVector(200, 0, 0));
	groundBox12->SetRelativeLocation(FVector(200, 100, 0));
	groundBox13->SetRelativeLocation(FVector(200, 200, 0));
	groundBox14->SetRelativeLocation(FVector(200, 300, 0));
	groundBox15->SetRelativeLocation(FVector(200, 400, 0));
	groundBox16->SetRelativeLocation(FVector(300, 0, 0));
	groundBox17->SetRelativeLocation(FVector(300, 100, 0));
	groundBox18->SetRelativeLocation(FVector(300, 200, 0));
	groundBox19->SetRelativeLocation(FVector(300, 300, 0));
	groundBox20->SetRelativeLocation(FVector(300, 400, 0));
	groundBox21->SetRelativeLocation(FVector(400, 0, 0));
	groundBox22->SetRelativeLocation(FVector(400, 100, 0));
	groundBox23->SetRelativeLocation(FVector(400, 200, 0));
	groundBox24->SetRelativeLocation(FVector(400, 300, 0));
	groundBox25->SetRelativeLocation(FVector(400, 400, 0));
	groundBox26->SetRelativeLocation(FVector(500, 0, 0));
	groundBox27->SetRelativeLocation(FVector(500, 100, 0));
	groundBox28->SetRelativeLocation(FVector(500, 200, 0));
	groundBox29->SetRelativeLocation(FVector(500, 300, 0));
	groundBox30->SetRelativeLocation(FVector(500, 400, 0));
	groundBox31->SetRelativeLocation(FVector(600, 0, 0));
	groundBox32->SetRelativeLocation(FVector(600, 100, 0));
	groundBox33->SetRelativeLocation(FVector(600, 200, 0));
	groundBox34->SetRelativeLocation(FVector(600, 300, 0));
	groundBox35->SetRelativeLocation(FVector(600, 400, 0));
	groundBox36->SetRelativeLocation(FVector(700, 0, 0));
	groundBox37->SetRelativeLocation(FVector(700, 100, 0));
	groundBox38->SetRelativeLocation(FVector(700, 200, 0));
	groundBox39->SetRelativeLocation(FVector(700, 300, 0));
	groundBox40->SetRelativeLocation(FVector(700, 400, 0));
	groundBox41->SetRelativeLocation(FVector(800, 0, 0));
	groundBox42->SetRelativeLocation(FVector(800, 100, 0));
	groundBox43->SetRelativeLocation(FVector(800, 200, 0));
	groundBox44->SetRelativeLocation(FVector(800, 300, 0));
	groundBox45->SetRelativeLocation(FVector(800, 400, 0));
	groundBox46->SetRelativeLocation(FVector(900, 0, 0));
	groundBox47->SetRelativeLocation(FVector(900, 100, 0));
	groundBox48->SetRelativeLocation(FVector(900, 200, 0));
	groundBox49->SetRelativeLocation(FVector(900, 300, 0));
	groundBox50->SetRelativeLocation(FVector(900, 400, 0));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/Meshes/SM_PathFindingGround.SM_PathFindingGround'"));
	if (tempMesh.Succeeded())
	{
		groundBox1->SetStaticMesh(tempMesh.Object);
		groundBox2->SetStaticMesh(tempMesh.Object);
		groundBox3->SetStaticMesh(tempMesh.Object);
		groundBox4->SetStaticMesh(tempMesh.Object);
		groundBox5->SetStaticMesh(tempMesh.Object);
		groundBox6->SetStaticMesh(tempMesh.Object);
		groundBox7->SetStaticMesh(tempMesh.Object);
		groundBox8->SetStaticMesh(tempMesh.Object);
		groundBox9->SetStaticMesh(tempMesh.Object);
		groundBox10->SetStaticMesh(tempMesh.Object);
		groundBox11->SetStaticMesh(tempMesh.Object);
		groundBox12->SetStaticMesh(tempMesh.Object);
		groundBox13->SetStaticMesh(tempMesh.Object);
		groundBox14->SetStaticMesh(tempMesh.Object);
		groundBox15->SetStaticMesh(tempMesh.Object);
		groundBox16->SetStaticMesh(tempMesh.Object);
		groundBox17->SetStaticMesh(tempMesh.Object);
		groundBox18->SetStaticMesh(tempMesh.Object);
		groundBox19->SetStaticMesh(tempMesh.Object);
		groundBox20->SetStaticMesh(tempMesh.Object);
		groundBox21->SetStaticMesh(tempMesh.Object);
		groundBox22->SetStaticMesh(tempMesh.Object);
		groundBox23->SetStaticMesh(tempMesh.Object);
		groundBox24->SetStaticMesh(tempMesh.Object);
		groundBox25->SetStaticMesh(tempMesh.Object);
		groundBox26->SetStaticMesh(tempMesh.Object);
		groundBox27->SetStaticMesh(tempMesh.Object);
		groundBox28->SetStaticMesh(tempMesh.Object);
		groundBox29->SetStaticMesh(tempMesh.Object);
		groundBox30->SetStaticMesh(tempMesh.Object);
		groundBox31->SetStaticMesh(tempMesh.Object);
		groundBox32->SetStaticMesh(tempMesh.Object);
		groundBox33->SetStaticMesh(tempMesh.Object);
		groundBox34->SetStaticMesh(tempMesh.Object);
		groundBox35->SetStaticMesh(tempMesh.Object);
		groundBox36->SetStaticMesh(tempMesh.Object);
		groundBox37->SetStaticMesh(tempMesh.Object);
		groundBox38->SetStaticMesh(tempMesh.Object);
		groundBox39->SetStaticMesh(tempMesh.Object);
		groundBox40->SetStaticMesh(tempMesh.Object);
		groundBox41->SetStaticMesh(tempMesh.Object);
		groundBox42->SetStaticMesh(tempMesh.Object);
		groundBox43->SetStaticMesh(tempMesh.Object);
		groundBox44->SetStaticMesh(tempMesh.Object);
		groundBox45->SetStaticMesh(tempMesh.Object);
		groundBox46->SetStaticMesh(tempMesh.Object);
		groundBox47->SetStaticMesh(tempMesh.Object);
		groundBox48->SetStaticMesh(tempMesh.Object);
		groundBox49->SetStaticMesh(tempMesh.Object);
		groundBox50->SetStaticMesh(tempMesh.Object);
	}
}

void APuzzleRoomThreePathFinding::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(hd, FTimerDelegate::CreateLambda([&]()
	{
		if (regularlyUpCount % 2 == 0)
		{
			PickBoxRandomly(NumberOfPopUpBox);
		}
		MovingTrigger();
		regularlyUpCount++;
	}), 4, true);
}

void APuzzleRoomThreePathFinding::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsMoving)
	{
		MovingFunctionAtTick(DeltaSeconds);
	}
}
void APuzzleRoomThreePathFinding::PickBoxRandomly(int number)
{
	// 초기화
	groundBoxArray.Empty();
	selectedBoxIndexArray.Empty();
	AddAllBoxCompToArray();

	// 시작과 끝점을 제외하고
	groundBoxArray.Remove(groundBox1);
	groundBoxArray.Remove(groundBox50);

	for(int i = 0; i < number; i++)
	{
		// 랜덤하게 골라서
		int idx = FMath::RandRange(0, groundBoxArray.Num() - 1);

		// 중복된거 선택되면 다시
		if (selectedBoxIndexArray.Contains(idx))
		{
			i--;
			continue;
		}
		selectedBoxIndexArray.Add(idx);
	}
}

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

void APuzzleRoomThreePathFinding::AddAllBoxCompToArray()
{
	groundBoxArray = {
		groundBox1,
		groundBox2,
		groundBox3,
		groundBox4,
		groundBox5,
		groundBox6,
		groundBox7,
		groundBox8,
		groundBox9,
		groundBox10,
		groundBox11,
		groundBox12,
		groundBox13,
		groundBox14,
		groundBox15,
		groundBox16,
		groundBox17,
		groundBox18,
		groundBox19,
		groundBox20,
		groundBox21,
		groundBox22,
		groundBox23,
		groundBox24,
		groundBox25,
		groundBox26,
		groundBox27,
		groundBox28,
		groundBox29,
		groundBox30,
		groundBox31,
		groundBox32,
		groundBox33,
		groundBox34,
		groundBox35,
		groundBox36,
		groundBox37,
		groundBox38,
		groundBox39,
		groundBox40,
		groundBox41,
		groundBox42,
		groundBox43,
		groundBox44,
		groundBox45,
		groundBox46,
		groundBox47,
		groundBox48,
		groundBox49,
		groundBox50,
	};
}
