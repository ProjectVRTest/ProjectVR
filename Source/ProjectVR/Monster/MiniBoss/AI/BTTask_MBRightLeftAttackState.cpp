// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBRightLeftAttackState.h"
#include "Headers/MiniBossAIHeader.h"


EBTNodeResult::Type UBTTask_MBRightLeftAttackState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			
		}
	}
	return EBTNodeResult::Succeeded;
}
