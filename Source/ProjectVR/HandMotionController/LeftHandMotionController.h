// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCharacter/GripEnum.h"
#include "LeftHandMotionController.generated.h"

UCLASS()
class PROJECTVR_API ALeftHandMotionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeftHandMotionController();

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
	//손주위에 있는 액터들을 판별할 원형 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* GrabSphere;
	//상호작용영역을 표시해줄 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USteamVRChaperoneComponent* SteamVRChaperone;
	//그립버튼을 눌럿는지 안눌럿는지 판별해줄 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool WantToGrip;
	//오른손에 붙인 검이 보이는지 안보이는지 판단해줄 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
		bool VisibleShieldFlag;
	//오른손에 물체가 닿앗는지 안닿앗는지 판단해줄 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool HandTouchActorFlag;
	//손에 붙일 액터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* AttachedActor;
	//손의 상태 (오픈, 쥘수 있는 상태, 쥔상태)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		E_HandState HandState;
	//모션컨트롤러 컴포넌트에 넣어줄 변수 (왼손인지 오른손인지 등)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
		EControllerHand Hand;
	//방패붙일 위치의 씬컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
		class USceneComponent* ShieldAttachScene;

	// 오너
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AMotionControllerCharacter* HandOwner;

	// 상호작용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWidgetInteractionComponent* interaction;

	//방패
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
		class APlayerShield* Shield;
	//포션가방붙일 위치의 씬컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PotionBag")
		class USceneComponent* PotionBagAttachScene;
	//포션가방
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PotionBag")
		class APotionBag* PotionBag;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* SphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Haptic")
		class UHapticFeedbackEffect_Base* VRHapticEffect;

	//그립버튼을 누를때 호출되는 함수
	UFUNCTION()
		void GrabActor();
	//그립버튼을 뗄때 호출되는 함수
	UFUNCTION()
		void ReleaseActor();
	//자기 자신주위에 액터가 있는지 확인해주는 함수
	UFUNCTION()
		AActor* GetActorNearHand();
	//왼손에 다른 액터가 부딪히면 호출되는 함수
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//손이 다른 액터에서 오버랩됀 후 빠져나올 때 호출
	UFUNCTION()
		void OnHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//열거형을 String으로 형변환해주는 함수
	UFUNCTION()
		FString GetEnumToString(EControllerHand Value);
};
