// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBParryingState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBParryingState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		AI->BBComponent->SetValueAsFloat(TEXT("AttackAnimationWaitTime"), 0);
		AI->BBComponent->SetValueAsFloat(TEXT("ReverseWaitTime"), 0);

		if (MiniBoss)
		{
			MiniBoss->IsAttack = false; //다시 공격할 수 있게 해줌	
			MiniBoss->AttackCompleteFlag = true;
			MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
			MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
			MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
			MiniBoss->CurrentState = EMiniBossState::Battle;		
		}
	}

	return EBTNodeResult::Succeeded;
}
