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
	//UPROPERTY(EditDefaultsOnly, Category = "ScrewDriver")
	//class UGrabComponent* grabComp;

	UPROPERTY()
	class AScrewDriver* driver;

	//드라이버에 나사가 닿으면
	UFUNCTION()
	void AttachtoDriver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//드라이버에서 나사가 떨어지면
	UFUNCTION()
	void DettachFromDriver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//닿았는가
	bool isAttaching;

	//회전하는가
	bool isRotating;
	void bRotating();

	//나사 시작 위치
	FVector startLoc;
	//나사가 나올만큼 다 나왔는가
	bool isEnoughRotated;
	void bEnoughCameOut();

	//드라이버의 이전 회전값
	float prevDriverRot;
	//변화되는 값
	float deltaRot;

	//나사가 이동한다
	void MoveScrew();
	//회전을 감지할 각도의 반올림
	int32 rotIntAngle = 0;

	//나사가 다 나온 거리
	double screwDist;
	
	//나사가 다 나왔다면 바닥에 떨어진다
	void CameOutScrew();
};
