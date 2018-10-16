// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossDeadState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossDeadState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

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
