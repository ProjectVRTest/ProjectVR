// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBRightLeftAttackState.h"
#include "Headers/MiniBossAIHeader.h"


EBTNodeResult::Type UBTTask_MBRightLeftAttackState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		float WaitTime;
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance");

			MiniBoss->StabFlag = false;
			if (Distance < 350.0f)
			{
				WaitTime = 1.0f;
				MiniBoss->AttackCompleteFlag = true;			
				MiniBoss->CurrentAttackState = EMiniBossAttackState::StabReady;
				AI->BBComponent->SetValueAsFloat(TEXT("AttackAnimationWaitTime"), WaitTime);
			}
			else
			{
				MiniBoss->IsAttack = false; //다시 공격할 수 있게 해줌				
				MiniBoss->CurrentState = EMiniBossState::Chase;
				MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
				MiniBoss->CurrentAttackState = EMiniBossAttackState::AttackReady;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
