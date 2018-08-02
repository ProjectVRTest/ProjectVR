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
		virtual void NativeUpdateAnimation(float DeltaSeconds) override; //�ִϸ��̼� �������Ʈ������ Tick

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandState")
		E_HandState HandState; //�޼� ��Ʈ�ѷ��� HandState���� �־��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandState")
		float GripState; // ���� �޼� ��Ʈ�ѷ��� HandState���� ���� GripState�� ����Ѵ�.
						 //HandState���� 0 �̸� GripState=0
						 //HandState���� 1 �̸� GripState=0.5
						 //HandState���� 2 �̸� GripState=1
						 //GripState�� ���� ���� �ִϸ��̼��� ����ȴ�.

	float CurrentHandState; //�޼� ��Ʈ�ѷ��� ���°��� �޾Ƽ� 2�� �������� �־��� ����
							//2�� ���� ������ ������ HandState�� �׸��� 3�� �̱� ����
	float InterpSpeed = 5.0f; //�����Ҷ� �ִ� ���� ���� �������� ������ ����
							  //���� �������� ������ �����ȴ�.						
};
