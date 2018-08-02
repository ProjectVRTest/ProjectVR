// Fill out your copyright notice in the Description page of Project Settings.

#include "LeftHandAnimInstance.h"
#include "HandMotionController/LeftHandMotionController.h"

void ULeftHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	AActor* Actor = GetOwningActor();//애니메이션 블루프린트를 사용하고 있는 액터를 반환해서 Actor에 저장한다.

	//액터가 있으면
	if (Actor != nullptr && Actor->IsValidLowLevelFast())
	{
		//구한 액터를 ALeftHandMotionController로 형변환해서 LeftHandController에 저장한다.
		ALeftHandMotionController* LeftHandController = Cast<ALeftHandMotionController>(Actor);

		if (LeftHandController) //형변환에 성공하면
		{
			HandState = LeftHandController->HandState;//LeftHandController이 가지고 있는 HandState를 가져와서 HandState에 넣어준다.
			CurrentHandState = (float)(HandState) / 2.0f;//손의 애니메이션을 출력해줄 GripState 값을 구해주기 위해 HandState값을 2로 나눈 값을 CurrentHandState에 넣는다.
			//앞에서 구한 CurrentHandState를 목표값으로 설정하고 GripState가 목표값까지 보간하는 값을 GripState에 넣어서 애니메이션을 출력해준다.
			GripState = FMath::FInterpConstantTo(GripState, CurrentHandState, DeltaSeconds, InterpSpeed);		
		}
	}
}



