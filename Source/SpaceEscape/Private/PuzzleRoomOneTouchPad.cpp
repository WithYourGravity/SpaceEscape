// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoomOneTouchPad.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

APuzzleRoomOneTouchPad::APuzzleRoomOneTouchPad()
{
	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TouchPanel Mesh"));
	SetRootComponent(meshComp);

	screenWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Screen Widget"));
	screenWidgetComp->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LTG/Assets/WallPanel/WallPanel_Cube.WallPanel_Cube'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
}

void APuzzleRoomOneTouchPad::BeginPlay()
{
	Super::BeginPlay();

}