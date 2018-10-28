// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCharacter/GripEnum.h"
#include "RightHandMotionController.generated.h"

UCLASS()
class PROJECTVR_API ARightHandMotionController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARightHandMotionController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//모션컨트롤러 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* MotionController;
	//출력해줄 손모양 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* HandMesh;


	//손 주위에 있는 물체를 감지할 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* GrabSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* OverlapSphere;

	//손의 상호작용 영역을 표현해줄 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USteamVRChaperoneComponent* SteamVRChaperone;

	//현재 물건을 쥐고 있는 상태인지 아닌지 판단해줄 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool WantToGrip;

	//손에 붙어 있는 액터를 저장해둘 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* AttachedActor;

	//오른손의 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		E_HandState HandState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EHandFormState HandFormState;

	//모션컨트롤러 열거형 변수 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hand")
		EControllerHand Hand;

	// 포션생성 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* PotionPosition;

	// 개가 붙는 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* AttachDogPosition;

	//검을 붙일 씬컴포넌트 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* SwordAttachScene;

	// 오너
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AMotionControllerCharacter* HandOwner;

	// 상호작용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWidgetInteractionComponent* interaction;

	//오른손에 붙일 검클래스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
		class APlayerSword* Sword;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Value")
		float SwayTime;
		float DefaultTime;

		int SwayCount;
		int TolerateCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Value")
		int stack;				// 개를 떨어뜨리기위한 스택
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Value")
		float Prelinear;				// 개를 떨어뜨리기위한 스택
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Value")
		float Currentlinear;				// 개를 떨어뜨리기위한 스택

	//오른손에 붙인 검이 보이는지 안보이는지 판단해줄 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
		bool VisibleSwordFlag;

	//오른손에 물체가 닿앗는지 안닿앗는지 판단해줄 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool HandTouchActorFlag;

	UPROPERTY()
		bool bisRightGrab;

	UPROPERTY()
		class APotion* Potion;

	//그립 버튼을 누를때 호출되는 함수
	UFUNCTION()
		void GrabActor();

	//그립 버튼을 뗄 때 호출되는 함수
	UFUNCTION()
		void ReleaseActor();

	//오른손 주위에 있는 액터들 중 가장 가까이 있는 액터를 반환할때 쓰이는 함수 ( 오른손 주위에 있는 액터를 반환 )
	UFUNCTION()
		AActor* GetActorNearHand();

	// 평시 상태 조건으로 
	UFUNCTION()
		void HandNomalState();

	// 오픈 상태 조건으로
	UFUNCTION()
		void HandOpenState();

	// 그랩 상태 조건으로
	UFUNCTION()
		void HandGrabState();

	//손이 다른 액터에 오버랩 될때 호출 되는 함수
	UFUNCTION()
		void OnHandBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//손이 다른 액터에서 오버랩 됫다가 빠져나올때 호출 되는 함수
	UFUNCTION()
		void OnHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//열거형을 스트링으로 변환해주는 함수
	UFUNCTION()
		FString GetEnumToString(EControllerHand Value);

	UPROPERTY()
		class ADog* AttachDog;

	// 손의 운동 값
	FVector HandCurrentPosistion, HandMoveDelta, HandPreviousPosistion, HandMoveVelocity;

};
