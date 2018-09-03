// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBSetState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBSetState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			MiniBoss->CurrentState = SetState;
			MiniBoss->CurrentAnimState = SetAnimState;
			MiniBoss->CurrentJumpState = SetJumpAnimState;
			MiniBoss->CurrentShortAttackState = SetShortAttackState;
		}
	}

	return EBTNodeResult::Succeeded;
}
