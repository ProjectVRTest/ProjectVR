// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_RoarState.h"

#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

EBTNodeResult::Type UBTTask_RoarState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			MiniBoss->CurrentState = EMiniBossState::Chase;
			MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}	
}
