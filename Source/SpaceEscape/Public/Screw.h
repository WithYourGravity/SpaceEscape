// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Screw.generated.h"

UCLASS()
class SPACEESCAPE_API AScrew : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScrew();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*컴포넌트 생성*/
	UPROPERTY(EditDefaultsOnly, Category = "Screw")
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, Category = "Screw")
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category = "Screw")
	class UArrowComponent* arrowComp;
	UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	class UGrabComponent* grabComp;

	UPROPERTY()
	class AScrewDriver* driver;

	//회전하는가
	bool isRotating;
	void bRotating();

	//나사 시작 위치
	FVector startLoc;
	//나사가 나올만큼 다 나왔는가
	bool isEnoughRotated;
	void bEnoughCameOut();

	//나사가 이동한다
	void MoveScrew();
	//회전을 감지할 각도의 반올림
	int32 rotIntAngle = 0;

	//시작위치로부터 나사가 뒤로 오지 못하게
	void NoMoveBwd();

	//나사가 다 나왔다면 바닥에 떨어진다
	void CameOutScrew();

	//deltaRot에서 나온 계수
	//int32 coeff = 0;
};
