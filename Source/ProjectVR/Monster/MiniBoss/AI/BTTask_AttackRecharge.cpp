// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AttackRecharge.h"
#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

EBTNodeResult::Type UBTTask_AttackRecharge::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			MiniBoss->CurrentAnimState = EMiniBossAnimState::Wait;
		}
	}
	return EBTNodeResult::Succeeded;
}
