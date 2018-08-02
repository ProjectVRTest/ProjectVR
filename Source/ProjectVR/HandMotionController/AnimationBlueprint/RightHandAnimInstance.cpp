// Fill out your copyright notice in the Description page of Project Settings.

#include "RightHandAnimInstance.h"
#include "HandMotionController/RightHandMotionController.h"

void URightHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	AActor* Actor = GetOwningActor();//�ִϸ��̼� �������Ʈ�� ����ϰ� �ִ� ���͸� ��ȯ�ؼ� Actor�� �����Ѵ�.

	//���Ͱ� ������
	if (Actor != nullptr && Actor->IsValidLowLevelFast()) 
	{
		//���� ���͸� ARightHandMotionController�� ����ȯ�ؼ� RightHandController�� �����Ѵ�.
		ARightHandMotionController* RightHandController = Cast<ARightHandMotionController>(Actor);

		if (RightHandController) //����ȯ�� �����ϸ�
		{
			HandState = RightHandController->HandState; //RightHandController�� ������ �ִ� HandState�� �����ͼ� HandState�� �־��ش�.
			CurrentHandState = (float)(HandState) / 2.0f; //���� �ִϸ��̼��� ������� GripState ���� �����ֱ� ���� HandState���� 2�� ���� ���� CurrentHandState�� �ִ´�.
			//�տ��� ���� CurrentHandState�� ��ǥ������ �����ϰ� GripState�� ��ǥ������ �����ϴ� ���� GripState�� �־ �ִϸ��̼��� ������ش�.
			GripState = FMath::FInterpConstantTo(GripState, CurrentHandState, DeltaSeconds, InterpSpeed);
		}
	}
}




