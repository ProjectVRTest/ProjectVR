// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBAttackReadyState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	//루트모션을 켜주는 행위는
	//루트본을 원점좌표로 설정해서 컬리전이 루트본의 움직임을 따라가게 해주는 행위로
	//루트모션 키기전,후 애니메이션 루트본 좌표의 차 만큼 컬리전을 움직여준다.
	
	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());
		
		if (MiniBoss)
		{
			int RandomPattern = FMath::RandRange(1, 3);
			
			MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::ShortAttackReady;
			MiniBoss->CurrentAnimState = EMiniBossAnimState::Attack;
			GLog->Log(FString::Printf(TEXT("어택 래디 스테이트 진입")));
		}
	}
	return EBTNodeResult::Succeeded;
}

