// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_DeadState.h"
#include "Monster/MiniBoss/MiniBossAIController.h"

EBTNodeResult::Type UBTTask_DeadState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AI->GetPawn()->Destroy();
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}	
}
