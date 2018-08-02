// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Mycharacter/GripEnum.h"
#include "LeftHandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVR_API ULeftHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION()
		virtual void NativeUpdateAnimation(float DeltaSeconds) override; //애니메이션 블루프린트에서의 Tick

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandState")
		E_HandState HandState; //왼손 컨트롤러의 HandState값을 넣어줄 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandState")
		float GripState; // 현재 왼손 컨트롤러의 HandState값을 토대로 GripState를 계산한다.
						 //HandState값이 0 이면 GripState=0
						 //HandState값이 1 이면 GripState=0.5
						 //HandState값이 2 이면 GripState=1
						 //GripState의 값에 따라 애니메이션이 실행된다.

	float CurrentHandState; //왼손 컨트롤러의 상태값을 받아서 2로 나눈값을 넣어줄 변수
							//2로 나눈 이유는 열거형 HandState의 항목이 3개 이기 때문
	float InterpSpeed = 5.0f; //보간할때 최대 몇초 동안 보간할지 정해줄 변수
							  //수가 낮을수록 느리게 보간된다.						
};
