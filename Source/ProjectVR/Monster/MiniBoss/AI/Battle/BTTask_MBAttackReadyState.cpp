// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBAttackReadyState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	//루트모션을 켜주는 행위는
	//루트본을 원점좌표로 설정해서 컬리전이 루트본의 움직임을 따라가게 해주는 행위로
	//루트모션 키기전,후 애니메이션 루트본 좌표의 차 만큼 컬리전을 움직여준다.
	
	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());
		
		if (MiniBoss)
		{
			int RandomPattern = FMath::RandRange(1, 15);
			
			if (RandomPattern > 0 && RandomPattern <8)
			{
				MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
				MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
				MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::ShortAttackReady;
				MiniBoss->CurrentAttackState = EMiniBossAttackState::ShortAttack;
			}
			else if (RandomPattern > 7 && RandomPattern < 12)
			{
				MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
				MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
				MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::TwoHandWidthReady;
				MiniBoss->CurrentAttackState = EMiniBossAttackState::WaveAttack;
			}
			else if (RandomPattern > 11)
			{
				MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
				MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
				MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::ComboAttackStart;
				MiniBoss->CurrentAttackState = EMiniBossAttackState::ComboAttack;
			}
			MiniBoss->CurrentAnimState = EMiniBossAnimState::Attack;
		}
	}
	return EBTNodeResult::Succeeded;
}

