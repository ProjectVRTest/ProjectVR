// Fill out your copyright notice in the Description page of Project Settings.

#include "LeftHandAnimInstance.h"
#include "HandMotionController/LeftHandMotionController.h"

void ULeftHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	AActor* Actor = GetOwningActor();//�ִϸ��̼� �������Ʈ�� ����ϰ� �ִ� ���͸� ��ȯ�ؼ� Actor�� �����Ѵ�.

	//���Ͱ� ������
	if (Actor != nullptr && Actor->IsValidLowLevelFast())
	{
		//���� ���͸� ALeftHandMotionController�� ����ȯ�ؼ� LeftHandController�� �����Ѵ�.
		ALeftHandMotionController* LeftHandController = Cast<ALeftHandMotionController>(Actor);

		if (LeftHandController) //����ȯ�� �����ϸ�
		{
			HandState = LeftHandController->HandState;//LeftHandController�� ������ �ִ� HandState�� �����ͼ� HandState�� �־��ش�.
			CurrentHandState = (float)(HandState) / 2.0f;//���� �ִϸ��̼��� ������� GripState ���� �����ֱ� ���� HandState���� 2�� ���� ���� CurrentHandState�� �ִ´�.
			//�տ��� ���� CurrentHandState�� ��ǥ������ �����ϰ� GripState�� ��ǥ������ �����ϴ� ���� GripState�� �־ �ִϸ��̼��� ������ش�.
			GripState = FMath::FInterpConstantTo(GripState, CurrentHandState, DeltaSeconds, InterpSpeed);		
		}
	}
}



