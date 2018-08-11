// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_JumpStart.h"

#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

EBTNodeResult::Type UBTTask_JumpStart::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			if (MiniBoss->CurrentAnimState == EMiniBossAnimState::JumpAttack)
			{
				if (MiniBoss->CurrentJumpState == EMiniBossJumpState::Idle)
				{
					MiniBoss->CurrentJumpState = EMiniBossJumpState::JumpStart;
				}
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
